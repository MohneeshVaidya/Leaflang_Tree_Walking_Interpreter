#include "Tools.hpp"

#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_literals;

void gen_ast_classes(const std::string& file_name, std::vector<std::string>&& scripts);

int main() {
    gen_ast_classes(
        "Expr"s,
        std::vector<std::string> {
            "TernaryExpr: ExprPtr condition, ExprPtr first, ExprPtr second"s,
            "BinaryExpr: TokenPtr oper, ExprPtr left, ExprPtr right"s,
            "UnaryExpr: TokenPtr oper, ExprPtr expr"s,
            "ExponentExpr: TokenPtr oper, ExprPtr left, ExprPtr right"s,
            "GroupExpr: ExprPtr expr"s,
            "PrimaryExpr: TokenPtr token"s,
        }
    );

    return 0;
}

void gen_hpp_file(const std::string& file_name, const std::vector<std::string>& scripts);
void gen_cpp_file(const std::string& file_name, const std::vector<std::string>& scripts);
void gen_visitor_hpp(const std::vector<std::string>& scripts);

void gen_ast_classes(const std::string& file_name, std::vector<std::string>&& scripts) {
    gen_hpp_file(file_name, scripts);
    gen_cpp_file(file_name, scripts);
    // gen_visitor_hpp(scripts);
}

void gen_hpp_file(const std::string& file_name, const std::vector<std::string>& scripts) {
    std::ofstream file_hpp { "../"s + file_name + ".hpp"s };

    if (file_hpp.is_open() == false) {
        std::cerr << std::format("Not able to open the file {}.hpp.\n", file_name);
        std::abort();
    }

    file_hpp << "#ifndef " << Tools::string_toupper(file_name) << "_HPP\n";
    file_hpp << "#define " << Tools::string_toupper(file_name) << "_HPP\n\n";

    file_hpp << "#include \"Token.hpp\"\n\n";

    file_hpp << "#include <memory>\n\n";

    file_hpp << std::format("class {};\n\n", file_name);

    file_hpp << std::format("using {}Ptr = std::shared_ptr<{}>;\n\n", file_name, file_name);

    file_hpp << "class " << file_name << " {\n";
    file_hpp << "public:\n";
    file_hpp << std::format("    // virtual {}Ptr accept() = 0;\n", file_name);
    file_hpp << "};\n\n";

    std::vector<std::string> ptr_aliases { };

    for (std::string_view script : scripts) {
        std::vector<std::string> parts { Tools::string_split(script, ":") };

        std::string_view class_name { parts.at(0) };
        std::string_view fields_str { parts.at(1) };

        ptr_aliases.push_back(std::format("using {}Ptr = std::shared_ptr<{}>;", class_name, class_name));

        file_hpp << "class " << class_name << " : public " << file_name << " {\n";
        file_hpp << "public:\n";

        for (std::string_view field : Tools::string_split(fields_str, ",")) {
            file_hpp << "    " << field << ";\n";
        }
        file_hpp << "\n";
        file_hpp << "    " << class_name << "(" << fields_str << ");\n";
        file_hpp << "};\n\n";
    }

    for (std::string_view ptr_alias : ptr_aliases) {
        file_hpp << ptr_alias << "\n";
    }
    file_hpp << "\n";

    file_hpp << "#endif";

    file_hpp.close();
}

void gen_cpp_file(const std::string& file_name, const std::vector<std::string>& scripts) {
    std::ofstream file_cpp { "../"s + file_name + ".cpp" };

    if (file_cpp.is_open() == false) {
        std::cerr << std::format("Not able to open file {}.cpp.\n", file_name);
        std::abort();
    }

    file_cpp << std::format("#include \"{}.hpp\"\n", file_name);
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

void gen_visitor_hpp(const std::vector<std::string>& scripts) {
    
}
