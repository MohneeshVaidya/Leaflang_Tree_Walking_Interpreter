export type TokenType = string

const PRINT: TokenType = "TOKEN_PRINT"
const PRINTLN: TokenType = "TOKEN_PRINTLN"
const LET: TokenType = "TOKEN_LET"
const CONST: TokenType = "TOKEN_CONST"
const IF: TokenType = "TOKEN_IF"
const ELSEIF: TokenType = "TOKEN_ELSEIF"
const ELSE: TokenType = "TOKEN_ELSE"
const FOR: TokenType = "TOKEN_FOR"
const BREAK: TokenType = "TOKEN_BREAK"
const CONTINUE: TokenType = "TOKEN_CONTINUE"
const FUNC: TokenType = "TOKEN_FUNC"
const RETURN: TokenType = "TOKEN_RETURN"
const CLASS: TokenType = "TOKEN_CLASS"
const CONSTRUCTOR: TokenType = "TOKEN_CONSTRUCTOR"
const EXTENDS: TokenType = "TOKEN_EXTENDS"
const THIS: TokenType = "TOKEN_THIS"
const SUPER: TokenType = "TOKEN_SUPER"
const AND: TokenType = "TOKEN_AND"
const OR: TokenType = "TOKEN_OR"
const XOR: TokenType = "TOKEN_XOR"
const TRUE: TokenType = "TOKEN_TRUE"
const FALSE: TokenType = "TOKEN_FALSE"
const IDENTIFIER: TokenType = "TOKEN_IDENTIFIER"
const STRING: TokenType = "TOKEN_STRING"
const FMT_STRING: TokenType = "TOKEN_FMT_STRING"
const NUMBER: TokenType = "TOKEN_NUMBER"
const NIL: TokenType = "TOKEN_NIL"
const EOF: TokenType = "TOKEN_EOF"
const NEWLINE: TokenType = "TOKEN_NEWLINE"

const PLUS: TokenType = "TOKEN_PLUS"
const MINUS: TokenType = "TOKEN_MINUS"
const STAR: TokenType = "TOKEN_STAR"
const SLASH: TokenType = "TOKEN_SLASH"
const PERCENT: TokenType = "TOKEN_PERCENT"
const STAR_STAR: TokenType = "TOKEN_STAR_STAR"
const EQUAL: TokenType = "TOKEN_EQUAL"
const EQUAL_EQUAL: TokenType = "TOKEN_EQUAL_EQUAL"
const BANG: TokenType = "TOKEN_BANG"
const BANG_EQUAL: TokenType = "TOKEN_BANG_EQUAL"
const LESSER: TokenType = "TOKEN_LESSER"
const LESSER_EQUAL: TokenType = "TOKEN_LESSER_EQUAL"
const GREATER: TokenType = "TOKEN_GREATER"
const GREATER_EQUAL: TokenType = "TOKEN_GREATER_EQUAL"
const EQUAL_GREATER: TokenType = "EQUAL_GREATER"

const SEMICOLON: TokenType = "TOKEN_SEMICOLON"
const COLON: TokenType = "TOKEN_COLON"
const COMMA: TokenType = "TOKEN_COMMA"
const LEFT_PAREN: TokenType = "TOKEN_LEFT_PAREN"
const RIGHT_PAREN: TokenType = "TOKEN_RIGHT_PAREN"
const LEFT_BRACE: TokenType = "TOKEN_LEFT_BRACE"
const RIGHT_BRACE: TokenType = "TOKEN_RIGHT_BRACE"
const DOT: TokenType = "TOKEN_DOT"
const QUESTION: TokenType = "TOKEN_QUESTION"

export default {
    PRINT,
    PRINTLN,
    LET,
    CONST,
    IF,
    ELSEIF,
    ELSE,
    FOR,
    BREAK,
    CONTINUE,
    FUNC,
    RETURN,
    CLASS,
    CONSTRUCTOR,
    EXTENDS,
    THIS,
    SUPER,
    AND,
    OR,
    XOR,
    TRUE,
    FALSE,
    IDENTIFIER,
    STRING,
    FMT_STRING,
    NUMBER,
    NIL,
    EOF,
    NEWLINE,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    STAR_STAR,
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    LESSER,
    LESSER_EQUAL,
    GREATER,
    GREATER_EQUAL,
    EQUAL_GREATER,

    SEMICOLON,
    COLON,
    COMMA,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    DOT,
    QUESTION,
}
