//
// Validates real example payloads against docs/schema/openrpc.json and
// docs/schema/feedback-events.schema.json using the actual json-schema-
// validator library, rather than just checking the schema files are
// well-formed JSON. Covers every request/response method in openrpc.json
// (parse, transform, sat, logical_consequence, truth_table, shutdown) and
// every notification def in feedback-events.schema.json.
//
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>

namespace {
nlohmann::json load_json_file(const std::string &path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("could not open schema file: " + path);
    }
    nlohmann::json j;
    in >> j;
    return j;
}

nlohmann::json sample_lexeme() {
    return {{"type", "Atom"}, {"token", "P"}, {"start", 0}, {"end", 1}};
}

nlohmann::json sample_node() {
    return {{"id", "3ddb3a23-8ac6-4725-b181-5409c3cebfd8"},
            {"type", "Atom"},
            {"lexeme", sample_lexeme()},
            {"children", nlohmann::json::array()}};
}

nlohmann::json sample_cursor() {
    return {{"current_node", nullptr}, {"tree_root", nullptr}};
}

// One example "params" payload per _NotificationTemplates/* shape that
// feedback-events.schema.json's Notification_* defs reference (there are
// far fewer distinct templates than defs, since most defs share one).
const std::unordered_map<std::string, nlohmann::json> &template_examples() {
    static const std::unordered_map<std::string, nlohmann::json> examples = {
        {"Empty", nlohmann::json::object()},
        {"SingleLexemeEvent", {{"lexeme", sample_lexeme()}}},
        {"CharacterEvent", {{"value", "P"}}},
        {"UnrecognisedOperatorEvent", {{"operator_text", "@"}, {"position", 3}}},
        {"SubscriptPositionEvent", {{"position", 2}}},
        {"UnexpectedSequenceEvent", {{"sequence", "xyz"}, {"position", 5}}},
        {"SingleNodeEvent", {{"node", sample_node()}}},
        {"InvalidPositionEvent", {{"node", sample_node()}, {"index", 2}}},
        {"SpawnedChildEvent",
         {{"parent", sample_node()}, {"child", sample_node()}}},
        {"AdoptNodeEvent",
         {{"parent", sample_node()}, {"child", sample_node()}}},
        {"DisconnectEvent", {{"node", sample_node()}, {"index", 0}}},
        {"TransformLexemeEvent",
         {{"node", sample_node()}, {"lexeme", sample_lexeme()}}},
        {"TransferNodeEvent",
         {{"source", sample_node()}, {"index", 1}, {"destination", sample_node()}}},
        {"AstErrorEvent", {{"cursor", sample_cursor()}}},
        {"PureAtomInParensEvent",
         {{"open_paren", sample_lexeme()},
          {"atom", sample_lexeme()},
          {"close_paren", sample_lexeme()}}},
        {"JoinEvent", {{"target", sample_node()}, {"source", sample_node()}}},
        {"CondenseChildEvent",
         {{"parent", sample_node()}, {"merged_child", sample_node()}}},
        {"NormalFormCheckEvent",
         {{"node", sample_node()}, {"is_normal_form", true}}},
        {"UnknownVariableEvent", {{"name", "Z"}}},
        {"VariableAssignmentEvent", {{"name", "P"}, {"value", true}}},
        {"SubexpressionResultEvent",
         {{"node_id", "3ddb3a23-8ac6-4725-b181-5409c3cebfd8"}, {"value", true}}},
        {"LiteralMappingEvent", {{"mapping", {{"P", 1}}}}},
        {"ClauseSetEvent", {{"clauses", nlohmann::json::array({{1, -2}})}}},
        {"LiteralEvent", {{"literal", 3}}},
        {"ClauseLiteralEvent", {{"clause", {1, 2}}, {"literal", 1}}},
        {"ClauseReductionEvent",
         {{"original", {1, 2}}, {"reduced", {2}}, {"literal", 1}}},
        {"ClausePairEvent", {{"first", {1}}, {"second", {-1}}}},
        {"ResolventEvent",
         {{"first", {1, 2}}, {"second", {-1, 3}}, {"pivot", 1}, {"resolvent", {2, 3}}}},
        {"ClauseEvent", {{"clause", {1, 2}}}},
        {"ClauseCountEvent", {{"count", 5}}},
        {"BranchingLiteralEvent", {{"literal", 2}, {"frequency", 3}}},
        {"BranchEvent", {{"literal", 2}, {"value", true}}},
        {"BoolResultEvent", {{"result", true}}},
    };
    return examples;
}

// Extracts {method_name -> template_name} for every "Notification_*" def in
// feedback-events.schema.json's $defs, by reading each def's own structure
// ("allOf":[{"$ref": ".../<TemplateName>"}, {"properties":{"method":
// {"const": "<method_name>"}}}]) directly, rather than hand-transcribing
// the 110+ defs into this file where they could silently drift out of sync
// with the schema. Only template_examples() above needs to be kept in sync
// (with the much smaller set of distinct templates) -- any def using a
// template with no registered example fails loudly via the ASSERT below,
// rather than being silently skipped.
std::vector<std::pair<std::string, std::string>>
extract_all_notifications(const nlohmann::json &feedback_events) {
    std::vector<std::pair<std::string, std::string>> result;
    for (const auto &[key, def] : feedback_events.at("$defs").items()) {
        if (key.rfind("Notification_", 0) != 0)
            continue;
        const auto &all_of = def.at("allOf");
        std::string ref = all_of.at(0).at("$ref").get<std::string>();
        std::string template_name = ref.substr(ref.find_last_of('/') + 1);
        std::string method = all_of.at(1)
                                  .at("properties")
                                  .at("method")
                                  .at("const")
                                  .get<std::string>();
        result.emplace_back(std::move(method), std::move(template_name));
    }
    return result;
}
} // namespace

class SchemaTest : public ::testing::Test {
  protected:
    nlohmann::json openrpc;
    nlohmann::json feedback_events;

    void SetUp() override {
        openrpc = load_json_file(std::string(SCHEMA_DIR) + "/openrpc.json");
        feedback_events =
            load_json_file(std::string(SCHEMA_DIR) + "/feedback-events.schema.json");
    }

    // Finds the entry in openrpc["methods"] named `name`.
    [[nodiscard]] const nlohmann::json &find_method(const std::string &name) const {
        for (const auto &method : openrpc.at("methods")) {
            if (method.at("name") == name)
                return method;
        }
        throw std::runtime_error("openrpc.json has no method named " + name);
    }

    // Builds a validator for one sub-schema of openrpc.json (a method's
    // params[0].schema or result.schema) by embedding openrpc's shared
    // `components` object into a wrapper root, so
    // "$ref": "#/components/schemas/..." resolves against it -- the
    // standard way to validate a fragment of a larger document with a
    // plain (non-OpenRPC-aware) JSON-Schema validator.
    //
    // The embedded key is "$defs", not "components", and every
    // "#/components/schemas/..." $ref is rewritten to "#/$defs/...": the
    // validator library only pre-registers (and so can safely resolve
    // cycles in) definitions nested under a literal "$defs" or
    // "definitions" key. Node is self-referential (children[].$ref ->
    // Node itself), and left under "components.schemas" as OpenRPC
    // structures it, compiling that schema recurses into itself forever
    // and crashes (verified via a minimal repro against this exact
    // library version) -- this rewrite is purely a test-harness
    // workaround for that, not a statement about openrpc.json's own
    // structure, which is fine as an OpenRPC document.
    [[nodiscard]] nlohmann::json_schema::json_validator
    validator_for(const nlohmann::json &sub_schema) const {
        nlohmann::json wrapper = sub_schema;
        wrapper["$defs"] = openrpc.at("components").at("schemas");
        std::string dumped = wrapper.dump();
        const std::string from = "#/components/schemas/";
        const std::string to = "#/$defs/";
        for (size_t pos = 0; (pos = dumped.find(from, pos)) != std::string::npos;
             pos += to.size())
            dumped.replace(pos, from.size(), to);
        wrapper = nlohmann::json::parse(dumped);

        // Uuid ("$defs"/"components.schemas") declares "format": "uuid", so
        // the validator needs a format checker registered up front (even
        // for instances that never touch that branch) or it refuses to
        // build the schema at all -- the library ships a default one.
        nlohmann::json_schema::json_validator validator(
            nullptr, nlohmann::json_schema::default_string_format_check);
        validator.set_root_schema(wrapper);
        return validator;
    }

    [[nodiscard]] nlohmann::json_schema::json_validator
    params_validator_for(const std::string &method_name) const {
        return validator_for(find_method(method_name).at("params").at(0).at("schema"));
    }

    [[nodiscard]] nlohmann::json_schema::json_validator
    result_validator_for(const std::string &method_name) const {
        return validator_for(find_method(method_name).at("result").at("schema"));
    }

    // feedback-events.schema.json is already a self-contained JSON-Schema
    // document (its own top-level "$ref" points at $defs.AnyFeedbackNotification),
    // so it needs no wrapper -- the whole document is the root schema.
    [[nodiscard]] nlohmann::json_schema::json_validator
    feedback_validator() const {
        nlohmann::json_schema::json_validator validator(
            nullptr, nlohmann::json_schema::default_string_format_check);
        validator.set_root_schema(feedback_events);
        return validator;
    }

    static bool is_valid(nlohmann::json_schema::json_validator &validator,
                         const nlohmann::json &instance) {
        try {
            validator.validate(instance);
            return true;
        } catch (const std::exception &) {
            return false;
        }
    }
};

// ---------------------------------------------------------------------
// openrpc.json: method inventory
// ---------------------------------------------------------------------

TEST_F(SchemaTest, OpenrpcDeclaresExactlyTheKnownMethods) {
    std::vector<std::string> names;
    for (const auto &method : openrpc.at("methods"))
        names.push_back(method.at("name").get<std::string>());

    EXPECT_THAT(names, ::testing::UnorderedElementsAre(
                            "parse", "transform", "sat", "logical_consequence",
                            "truth_table", "shutdown"));
}

// ---------------------------------------------------------------------
// openrpc.json: "parse"
// ---------------------------------------------------------------------

TEST_F(SchemaTest, ParseParamsAcceptsAMinimalRequest) {
    auto validator = params_validator_for("parse");
    EXPECT_TRUE(is_valid(validator, {{"formula", "P \\wedge Q"}}));
}

TEST_F(SchemaTest, ParseParamsAcceptsAnExplicitStrictFlag) {
    auto validator = params_validator_for("parse");
    EXPECT_TRUE(is_valid(validator, {{"formula", "(P \\wedge Q)"}, {"strict", true}}));
}

TEST_F(SchemaTest, ParseParamsRejectsAMissingFormula) {
    auto validator = params_validator_for("parse");
    EXPECT_FALSE(is_valid(validator, nlohmann::json::object()));
}

TEST_F(SchemaTest, ParseParamsRejectsAnUnknownProperty) {
    auto validator = params_validator_for("parse");
    EXPECT_FALSE(is_valid(validator, {{"formula", "P"}, {"target", "cnf"}}));
}

TEST_F(SchemaTest, ParseResultAcceptsATreeOrNull) {
    auto validator = result_validator_for("parse");
    EXPECT_TRUE(is_valid(validator, {{"tree", sample_node()}}));
    EXPECT_TRUE(is_valid(validator, {{"tree", nullptr}}));
}

TEST_F(SchemaTest, ParseResultRejectsAMissingTree) {
    auto validator = result_validator_for("parse");
    EXPECT_FALSE(is_valid(validator, nlohmann::json::object()));
}

TEST_F(SchemaTest, ParseResultRejectsAWrongTypeForTree) {
    auto validator = result_validator_for("parse");
    EXPECT_FALSE(is_valid(validator, {{"tree", "not a node"}}));
}

// ---------------------------------------------------------------------
// openrpc.json: "transform"
// ---------------------------------------------------------------------

TEST_F(SchemaTest, TransformParamsAcceptsEachTarget) {
    auto validator = params_validator_for("transform");
    EXPECT_TRUE(is_valid(validator, {{"formula", "P"}, {"target", "nnf"}}));
    EXPECT_TRUE(is_valid(validator, {{"formula", "P"}, {"target", "cnf"}}));
    EXPECT_TRUE(is_valid(validator, {{"formula", "P"}, {"target", "dnf"}}));
    EXPECT_TRUE(is_valid(validator, {{"formula", "P"}})); // target is optional
}

TEST_F(SchemaTest, TransformParamsRejectsAnUnknownTarget) {
    auto validator = params_validator_for("transform");
    EXPECT_FALSE(is_valid(validator, {{"formula", "P"}, {"target", "xnf"}}));
}

TEST_F(SchemaTest, TransformParamsRejectsAMissingFormula) {
    auto validator = params_validator_for("transform");
    EXPECT_FALSE(is_valid(validator, {{"target", "cnf"}}));
}

TEST_F(SchemaTest, TransformResultAcceptsATreeOrNull) {
    auto validator = result_validator_for("transform");
    EXPECT_TRUE(is_valid(validator, {{"tree", sample_node()}}));
    EXPECT_TRUE(is_valid(validator, {{"tree", nullptr}}));
}

TEST_F(SchemaTest, TransformResultRejectsAnExtraProperty) {
    auto validator = result_validator_for("transform");
    EXPECT_FALSE(is_valid(validator, {{"tree", nullptr}, {"target", "cnf"}}));
}

// ---------------------------------------------------------------------
// openrpc.json: "sat"
// ---------------------------------------------------------------------

TEST_F(SchemaTest, SatParamsAcceptsEachAlgorithm) {
    auto validator = params_validator_for("sat");
    for (const std::string &algorithm : {"dp", "dpll_mcl", "dpll_rand", "resolution"})
        EXPECT_TRUE(is_valid(validator, {{"formula", "P"}, {"algorithm", algorithm}}))
            << "algorithm: " << algorithm;
    EXPECT_TRUE(is_valid(validator, {{"formula", "P"}})); // algorithm is optional
}

TEST_F(SchemaTest, SatParamsRejectsAnUnknownAlgorithm) {
    auto validator = params_validator_for("sat");
    EXPECT_FALSE(is_valid(validator, {{"formula", "P"}, {"algorithm", "quantum"}}));
}

TEST_F(SchemaTest, SatParamsRejectsAMissingFormula) {
    auto validator = params_validator_for("sat");
    EXPECT_FALSE(is_valid(validator, {{"algorithm", "dp"}}));
}

TEST_F(SchemaTest, SatResultAcceptsEachState) {
    auto validator = result_validator_for("sat");
    for (const std::string &state : {"SAT", "UNSAT", "UNKNOWN"})
        EXPECT_TRUE(is_valid(validator, {{"state", state},
                                         {"mapping", {{"P", 1}}},
                                         {"clauses", nlohmann::json::array({{1}})}}))
            << "state: " << state;
}

TEST_F(SchemaTest, SatResultRejectsAnUnknownState) {
    auto validator = result_validator_for("sat");
    EXPECT_FALSE(is_valid(validator, {{"state", "MAYBE"},
                                      {"mapping", {{"P", 1}}},
                                      {"clauses", nlohmann::json::array({{1}})}}));
}

TEST_F(SchemaTest, SatResultRejectsAWrongTypeForClauses) {
    auto validator = result_validator_for("sat");
    EXPECT_FALSE(is_valid(
        validator, {{"state", "SAT"}, {"mapping", {{"P", 1}}}, {"clauses", "none"}}));
}

// ---------------------------------------------------------------------
// openrpc.json: "logical_consequence"
// ---------------------------------------------------------------------

TEST_F(SchemaTest, LogicalConsequenceParamsAcceptsAMinimalRequest) {
    auto validator = params_validator_for("logical_consequence");
    EXPECT_TRUE(is_valid(validator, {{"formula", "P \\models P"}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsAcceptsAnExplicitStrictFlag) {
    auto validator = params_validator_for("logical_consequence");
    EXPECT_TRUE(is_valid(
        validator, {{"formula", "A \\wedge (A \\implies B) \\models B"}, {"strict", true}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsRejectsAMissingFormula) {
    auto validator = params_validator_for("logical_consequence");
    EXPECT_FALSE(is_valid(validator, {{"strict", false}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsRejectsAnUnknownProperty) {
    auto validator = params_validator_for("logical_consequence");
    EXPECT_FALSE(is_valid(validator, {{"formula", "P \\models P"}, {"algorithm", "dp"}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultAcceptsTrueAndFalse) {
    auto validator = result_validator_for("logical_consequence");
    EXPECT_TRUE(is_valid(validator, {{"result", true}}));
    EXPECT_TRUE(is_valid(validator, {{"result", false}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultRejectsAWrongType) {
    auto validator = result_validator_for("logical_consequence");
    EXPECT_FALSE(is_valid(validator, {{"result", "yes"}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultRejectsAnExtraProperty) {
    auto validator = result_validator_for("logical_consequence");
    EXPECT_FALSE(is_valid(validator, {{"result", true}, {"mapping", {}}}));
}

// ---------------------------------------------------------------------
// openrpc.json: "truth_table"
// ---------------------------------------------------------------------

TEST_F(SchemaTest, TruthTableParamsAcceptsAMinimalRequest) {
    auto validator = params_validator_for("truth_table");
    EXPECT_TRUE(is_valid(validator, {{"formula", "P \\wedge Q"}}));
}

TEST_F(SchemaTest, TruthTableParamsRejectsAMissingFormula) {
    auto validator = params_validator_for("truth_table");
    EXPECT_FALSE(is_valid(validator, nlohmann::json::object()));
}

TEST_F(SchemaTest, TruthTableResultAcceptsHeadersAndRows) {
    auto validator = result_validator_for("truth_table");
    EXPECT_TRUE(is_valid(validator, {{"headers", {"P", "Q", "P \\wedge Q"}},
                                     {"rows", nlohmann::json::array({
                                                  {true, true, true},
                                                  {true, false, false},
                                              })}}));
}

TEST_F(SchemaTest, TruthTableResultRejectsAMissingRows) {
    auto validator = result_validator_for("truth_table");
    EXPECT_FALSE(is_valid(validator, {{"headers", {"P"}}}));
}

// ---------------------------------------------------------------------
// openrpc.json: "shutdown" -- takes no params (its own "params" is an
// empty content-descriptor array), so only its result is validated here.
// ---------------------------------------------------------------------

TEST_F(SchemaTest, ShutdownResultAcceptsNoMessageOrAStringMessage) {
    auto validator = result_validator_for("shutdown");
    EXPECT_TRUE(is_valid(validator, nlohmann::json::object()));
    EXPECT_TRUE(is_valid(validator, {{"message", "bye"}}));
}

TEST_F(SchemaTest, ShutdownResultRejectsAWrongTypeForMessage) {
    auto validator = result_validator_for("shutdown");
    EXPECT_FALSE(is_valid(validator, {{"message", 5}}));
}

TEST_F(SchemaTest, ShutdownResultRejectsAnUnknownProperty) {
    auto validator = result_validator_for("shutdown");
    EXPECT_FALSE(is_valid(validator, {{"foo", "bar"}}));
}

// ---------------------------------------------------------------------
// feedback-events.schema.json
// ---------------------------------------------------------------------

TEST_F(SchemaTest, FeedbackEventsAcceptsEveryDocumentedNotification) {
    auto validator = feedback_validator();
    const auto &examples = template_examples();
    auto notifications = extract_all_notifications(feedback_events);

    // Sanity-checks the extraction itself found a realistic number of
    // defs, so a change to the def naming/shape convention that broke
    // extract_all_notifications silently (returning an empty/tiny list)
    // shows up here instead of this test quietly validating nothing.
    ASSERT_GE(notifications.size(), 100u);

    for (const auto &[method, template_name] : notifications) {
        auto it = examples.find(template_name);
        ASSERT_NE(it, examples.end())
            << "no example registered in template_examples() for template \""
            << template_name << "\" (used by notification \"" << method << "\")";

        nlohmann::json notification = {
            {"jsonrpc", "2.0"}, {"method", method}, {"params", it->second}};
        EXPECT_TRUE(is_valid(validator, notification)) << "method: " << method;
    }
}

TEST_F(SchemaTest, FeedbackEventsRejectsAnUnknownMethodName) {
    auto validator = feedback_validator();

    EXPECT_FALSE(is_valid(
        validator,
        {{"jsonrpc", "2.0"},
         {"method", "logical_consequence/didExplode"},
         {"params", nlohmann::json::object()}}));
}

TEST_F(SchemaTest, FeedbackEventsRejectsAFinalisedWithResultMissingItsField) {
    auto validator = feedback_validator();

    EXPECT_FALSE(is_valid(
        validator,
        {{"jsonrpc", "2.0"},
         {"method", "logical_consequence/finalised_with_result"},
         {"params", nlohmann::json::object()}}));
}

TEST_F(SchemaTest, FeedbackEventsRejectsAWrongCategoryPayloadForAKnownMethod) {
    // "sat/didFindUnitLiteral" needs a LiteralEvent payload, not an Empty
    // one -- confirms the discriminator actually ties method name to its
    // own specific payload shape, not just any shape in the anyOf.
    auto validator = feedback_validator();

    EXPECT_FALSE(is_valid(
        validator,
        {{"jsonrpc", "2.0"},
         {"method", "sat/didFindUnitLiteral"},
         {"params", nlohmann::json::object()}}));
}

TEST_F(SchemaTest, FeedbackEventsRejectsAMissingJsonrpcField) {
    auto validator = feedback_validator();

    EXPECT_FALSE(is_valid(
        validator, {{"method", "lexer/didStart"}, {"params", nlohmann::json::object()}}));
}
