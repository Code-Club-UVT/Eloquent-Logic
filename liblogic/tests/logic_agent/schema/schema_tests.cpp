//
// Validates real example payloads against docs/schema/openrpc.json and
// docs/schema/feedback-events.schema.json using the actual json-schema-
// validator library, rather than just checking the schema files are
// well-formed JSON. Focuses on the new "logical_consequence" method/DTOs,
// plus one pre-existing-method sanity case as a tripwire that the harness
// itself (in particular, the openrpc.json sub-schema-validation technique
// below) is set up correctly.
//
#include <gtest/gtest.h>

#include <fstream>
#include <stdexcept>
#include <string>

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
    [[nodiscard]] nlohmann::json_schema::json_validator
    validator_for(const nlohmann::json &sub_schema) const {
        nlohmann::json wrapper = sub_schema;
        wrapper["components"] = openrpc.at("components");
        // Uuid ("$defs"/"components.schemas") declares "format": "uuid", so
        // the validator needs a format checker registered up front (even
        // for instances that never touch that branch) or it refuses to
        // build the schema at all -- the library ships a default one.
        nlohmann::json_schema::json_validator validator(
            nullptr, nlohmann::json_schema::default_string_format_check);
        validator.set_root_schema(wrapper);
        return validator;
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

TEST_F(SchemaTest, LogicalConsequenceParamsAcceptsAMinimalRequest) {
    auto validator =
        validator_for(find_method("logical_consequence").at("params").at(0).at("schema"));

    EXPECT_TRUE(is_valid(validator, {{"formula", "P \\models P"}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsAcceptsAnExplicitStrictFlag) {
    auto validator =
        validator_for(find_method("logical_consequence").at("params").at(0).at("schema"));

    EXPECT_TRUE(is_valid(
        validator, {{"formula", "A \\wedge (A \\implies B) \\models B"}, {"strict", true}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsRejectsAMissingFormula) {
    auto validator =
        validator_for(find_method("logical_consequence").at("params").at(0).at("schema"));

    EXPECT_FALSE(is_valid(validator, {{"strict", false}}));
}

TEST_F(SchemaTest, LogicalConsequenceParamsRejectsAnUnknownProperty) {
    auto validator =
        validator_for(find_method("logical_consequence").at("params").at(0).at("schema"));

    EXPECT_FALSE(
        is_valid(validator, {{"formula", "P \\models P"}, {"algorithm", "dp"}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultAcceptsTrueAndFalse) {
    auto validator = validator_for(find_method("logical_consequence").at("result").at("schema"));

    EXPECT_TRUE(is_valid(validator, {{"result", true}}));
    EXPECT_TRUE(is_valid(validator, {{"result", false}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultRejectsAWrongType) {
    auto validator = validator_for(find_method("logical_consequence").at("result").at("schema"));

    EXPECT_FALSE(is_valid(validator, {{"result", "yes"}}));
}

TEST_F(SchemaTest, LogicalConsequenceResultRejectsAnExtraProperty) {
    auto validator = validator_for(find_method("logical_consequence").at("result").at("schema"));

    EXPECT_FALSE(is_valid(validator, {{"result", true}, {"mapping", {}}}));
}

TEST_F(SchemaTest, FeedbackEventsAcceptsEveryLogicalConsequenceNotification) {
    auto validator = feedback_validator();

    EXPECT_TRUE(is_valid(
        validator,
        {{"jsonrpc", "2.0"}, {"method", "logical_consequence/didStart"}, {"params", nlohmann::json::object()}}));
    EXPECT_TRUE(is_valid(
        validator,
        {{"jsonrpc", "2.0"}, {"method", "logical_consequence/didFinish"}, {"params", nlohmann::json::object()}}));
    EXPECT_TRUE(is_valid(
        validator,
        {{"jsonrpc", "2.0"},
         {"method", "logical_consequence/not_logical_equivalence"},
         {"params", nlohmann::json::object()}}));
    EXPECT_TRUE(is_valid(
        validator,
        {{"jsonrpc", "2.0"},
         {"method", "logical_consequence/finalised_with_result"},
         {"params", {{"result", true}}}}));
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

// Sanity check independent of anything new: a pre-existing method's result
// schema still validates a real example the same way, confirming the
// sub-schema-with-embedded-components technique itself is sound and not
// coincidentally only working for the schema this change added.
TEST_F(SchemaTest, PreExistingSatResultSchemaStillValidatesARealExample) {
    auto validator = validator_for(find_method("sat").at("result").at("schema"));

    EXPECT_TRUE(is_valid(validator, {{"state", "SAT"},
                                     {"mapping", {{"P", 1}}},
                                     {"clauses", {{1}}}}));
    EXPECT_FALSE(is_valid(validator, {{"state", "MAYBE"},
                                      {"mapping", {{"P", 1}}},
                                      {"clauses", {{1}}}}));
}
