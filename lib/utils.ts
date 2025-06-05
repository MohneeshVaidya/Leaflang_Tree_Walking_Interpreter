import { createInterface } from "node:readline/promises"
import IObj from "./object"

const readInput = async (text: string) => {
    const reader = createInterface({
        input: process.stdin,
        output: process.stdout,
    })
    const input = await reader.question(text)
    reader.close()
    return input
}

const isSpace = (char: string) => {
    return [" ", "\t", "\r", "\n"].includes(char)
}

const isNumeric = (char: string) => {
    return (
        ("0".codePointAt(0) as number) <= (char.codePointAt(0) as number) &&
        (char.codePointAt(0) as number) <= ("9".codePointAt(0) as number)
    )
}

const isAlphabetic = (char: string) => {
    return (
        (("a".codePointAt(0) as number) <= (char.codePointAt(0) as number) &&
            (char.codePointAt(0) as number) <=
                ("z".codePointAt(0) as number)) ||
        (("A".codePointAt(0) as number) <= (char.codePointAt(0) as number) &&
            (char.codePointAt(0) as number) <=
                ("Z".codePointAt(0) as number)) ||
        char === "_"
    )
}

const isAlphaNumeric = (char: string) => {
    return isAlphabetic(char) || isNumeric(char)
}

const isTruthy = (value: IObj) => {
    return !!value.value()
}

export default {
    readInput,
    isSpace,
    isNumeric,
    isAlphabetic,
    isAlphaNumeric,
    isTruthy,
}
