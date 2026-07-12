parser grammar LogicParse;
options { tokenVocab=LogicLex; }

program
    : (strict|relaxed) EOF
    ;

atomic_expression: VARIABLE;
exp_begin: EXP_BEGIN;
exp_end: EXP_END;

strict: atomic_expression
    | exp_begin negation_operator strict exp_end
    | exp_begin strict and_operator strict exp_end
    | exp_begin strict or_operator strict exp_end
    | exp_begin strict implies_operator strict exp_end
    | exp_begin strict iff_operator strict exp_end ;

negation_operator: NEGATION;
and_operator: AND;
or_operator:  OR;
implies_operator:  IMPLIES;
iff_operator: IFF;

// quick 'n dirty fix for a negation bug
negated_expression
    : atomic_expression
    | exp_begin relaxedInnerExpr exp_end
    ;
// Separate relaxed expression to prevent EXP_BEGIN VARIABLE EXP_END
relaxed
    : negation_operator negated_expression
//    : negation_operator relaxed
    | <assoc=right> relaxed and_operator relaxed
    | <assoc=right> relaxed or_operator relaxed
    | <assoc=right> relaxed implies_operator relaxed
    | <assoc=right> relaxed iff_operator relaxed
    | exp_begin relaxedInnerExpr exp_end
    | atomic_expression ;

// Define relaxedInnerExpr to exclude VARIABLE directly
relaxedInnerExpr
    : <assoc=right> relaxed and_operator relaxed #NestedAndExpr
    | <assoc=right> relaxed or_operator relaxed #NestedOrExpr
    | <assoc=right> relaxed implies_operator relaxed #NestedImpliesExpr
    | <assoc=right> relaxed iff_operator relaxed #NestedIffExpr
    | negation_operator relaxed #NestedNotExpr
    | exp_begin relaxedInnerExpr exp_end #FurtherNestedExpr
    ;