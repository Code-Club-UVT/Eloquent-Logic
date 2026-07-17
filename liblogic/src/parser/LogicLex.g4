lexer grammar LogicLex;


EXP_BEGIN   : '(' ;
EXP_END     : ')' ;

NEGATION    : '\\\\neg' ;

AND     : '\\\\wedge';
OR      : '\\\\vee';
IMPLIES : '\\\\implies';
IFF     : '\\\\iff';

// this may need some work
VARIABLE    : '\\\\top'     // TAUTOLOGY
            | '\\\\bot'     // CONTRADICTION
            | [a-zA-Z]
            | [a-zA-Z] '_' [0-9]
            | [a-zA-Z] '_{' [0-9]+ '}'
            ;
WS: [ \t\n\r\f]+ -> skip ;