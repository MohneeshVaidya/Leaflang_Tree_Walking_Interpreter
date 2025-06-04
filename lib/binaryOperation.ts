import LeafError from "./error"
import IObj, { ObjBool, ObjNumber, ObjString } from "./object"
import Token from "./token"
import utils from "./utils"

const binaryOperation = (oper: Token, left: IObj, right: IObj) => {
    try {
        return operation[oper.lexeme()](left, right)
    } catch (err) {
        LeafError.getInstance().throwRunTimeError(oper.line(), err.message)
    }
}

export default binaryOperation

const operation = {
    and: (left: IObj, right: IObj) => {
        return ObjBool.createInstance(
            utils.isTruthy(left) && utils.isTruthy(right)
        )
    },

    or: (left: IObj, right: IObj) => {
        return ObjBool.createInstance(
            utils.isTruthy(left) || utils.isTruthy(right)
        )
    },

    "==": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() === right.value())
    },

    "!=": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() !== right.value())
    },

    "<": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() < right.value())
    },

    "<=": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() <= right.value())
    },

    ">": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() > right.value())
    },

    ">=": (left: IObj, right: IObj) => {
        return ObjBool.createInstance(left.value() >= right.value())
    },

    "+": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber) {
            if (right instanceof ObjNumber)
                return ObjNumber.createInstance(left.value() + right.value())
            if (right instanceof ObjString)
                return ObjString.createInstance(left.value() + right.value())
        } else if (left instanceof ObjString) {
            if (right instanceof ObjNumber)
                return ObjString.createInstance(left.value() + right.value())
            if (right instanceof ObjString)
                return ObjString.createInstance(left.value() + right.value())
        }
        throw new Error(
            "for operator '+', both operands should be of string type or number type;"
        )
    },

    "-": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber && right instanceof ObjNumber) {
            return ObjNumber.createInstance(left.value() - right.value())
        }
        throw new Error(
            "for '-' operator, both operands should be of number type;"
        )
    },

    "*": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber && right instanceof ObjNumber) {
            return ObjNumber.createInstance(left.value() * right.value())
        }
        throw new Error(
            "for '*' operator, both operands should be of number type;"
        )
    },

    "/": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber && right instanceof ObjNumber) {
            return ObjNumber.createInstance(left.value() / right.value())
        }
        throw new Error(
            "for '/' operator, both operands should be of number type;"
        )
    },

    "%": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber && right instanceof ObjNumber) {
            return ObjNumber.createInstance(left.value() % right.value())
        }
        throw new Error(
            "for '%' operator, both operands should be of number type;"
        )
    },

    "**": (left: IObj, right: IObj) => {
        if (left instanceof ObjNumber && right instanceof ObjNumber) {
            return ObjNumber.createInstance(left.value() ** right.value())
        }
        throw new Error(
            "for '**' operator, both operands should be of number type;"
        )
    },
}
