import { createInterface } from "node:readline/promises"

const reader = createInterface({
    input: process.stdin,
    output: process.stdout,
})

const readInput = async (text: string) => {
    return reader.question(text)
}

const isSpace = (char: string) => {
    return [" ", "\t", "\r"].includes(char)
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

export default {
    readInput,
    isSpace,
    isNumeric,
    isAlphabetic,
    isAlphaNumeric,
}
