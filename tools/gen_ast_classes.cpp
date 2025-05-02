#include "Tools.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_literals;

void gen_ast_classes(const std::string& file_name, std::vector<std::string>&& scripts);

int main() {
    gen_ast_classes(
        "Expression"s,
        std::vector<std::string> {
            "TernaryExpr: ExpressionPtr condition, ExpressionPtr first, ExpressionPtr second"s,
            "EqualityExpr: TokenPtr oper, ExpressionPtr left, ExpressionPtr right"s,
            "ComparisionExpr: TokenPtr oper, ExpressionPtr left, ExpressionPtr right"s,
            "TermExpr: TokenPtr oper, ExpressionPtr left, ExpressionPtr right"s,
            "FactorExpr: TokenPtr oper, ExpressionPtr left, ExpressionPtr right"s,
            "UnaryExpr: TokenPtr oper, ExpressionPtr expr"s,
            "ExponentExpr: ExpressionPtr left, ExpressionPtr right"s,
            "GroupExpr: ExpressionPtr expr"s,
            "PrimaryExpr: TokenPtr token"s,
        }
    );

    return 0;
}

void gen_ast_classes(const std::string& file_name, std::vector<std::string>&& scripts) {
    // Create ../Expression.hpp
    std::ofstream file_hpp { "../"s + file_name + ".hpp"s };

    if (file_hpp.is_open() == false) {
        std::cerr << "Not able to open the file Expression.hpp.\n";
        std::abort();
    }

    file_hpp << "#ifndef " << Tools::string_toupper(file_name) << "_HPP\n";
    file_hpp << "#define " << Tools::string_toupper(file_name) << "_HPP\n\n";

    file_hpp << "#include \"Token.hpp\"\n\n";

    file_hpp << "#include <memory>\n\n";

    file_hpp << "class Expression;\n\n";

    file_hpp << "using ExpressionPtr = std::shared_ptr<Expression>;\n\n";

    file_hpp << "class Expression {\n";
    file_hpp << "public:\n";
    file_hpp << "    // virtual ExpressionPtr accept() = 0;\n";
    file_hpp << "};\n\n";

    for (std::string_view script : scripts) {
        std::vector<std::string> parts { Tools::string_split(script, ":") };

        std::string_view class_name { parts.at(0) };
        std::string_view fields_str { parts.at(1) };

        file_hpp << "class " << class_name << " : public Expression {\n";
        file_hpp << "public:\n";

        for (std::string_view field : Tools::string_split(fields_str, ",")) {
            file_hpp << "    " << field << ";\n";
        }
        file_hpp << "\n";
        file_hpp << "    " << class_name << "(" << fields_str << ");\n";
        file_hpp << "};\n\n";
    }

    file_hpp << "#endif";

    file_hpp.close();

    // Create Expression.cpp

    std::ofstream file_cpp { "../"s + file_name + ".cpp" };

    if (file_cpp.is_open() == false) {
        std::cerr << "Not able to open file Expression.cpp.\n";
        std::abort();
    }

    file_cpp << "#include \"Expression.hpp\"\n";
    file_cpp << "#include \"Token.hpp\"\n\n";

    for (std::string_view script : scripts) {
        std::vector<std::string> parts { Tools::string_split(script, ":") };

        std::string_view class_name { parts.at(0) };
        std::string_view fields_str { parts.at(1) };

        file_cpp << class_name << "::" << class_name << "(" << fields_str << ") :\n";

        std::vector<std::string> field_decls { Tools::string_split(fields_str, ",") };

        for (uint32_t i { 0 }; i < field_decls.size(); i++) {
            std::string_view field_name { Tools::string_split(field_decls.at(i), " ").at(1) };

            file_cpp << "    " << field_name << " { " << field_name << " }";

            if (i < field_decls.size() - 1) {
                file_cpp << ",";
            }
            file_cpp << "\n";
        }
        file_cpp << "    {\n    }\n\n";
    }

    file_cpp.close();
}
