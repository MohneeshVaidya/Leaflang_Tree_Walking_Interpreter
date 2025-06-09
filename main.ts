import LeafError from "./lib/error"
import Interpreter from "./lib/interpreter"
import Parser from "./lib/parser"
import Token from "./lib/token"
import Tokenizer from "./lib/tokenizer"
import utils from "./lib/utils"
import fs from "node:fs/promises"

const argv = process.argv.slice(1)
const argc = argv.length

process.on("uncaughtException", (err) => {
    console.log(err)
    exit(1)
})

const main = async (argc: number, argv: string[]) => {
    if (argc === 2 && argv[1] === "--help") {
        printHelp()
        exit(0)
    }

    if (argc > 2) {
        printHelp()
        exit(1)
    }
    if (argc === 1) {
        await runRepl()
        exit(0)
    }

    await runFile(argv[1])
}

const printHelp = () => {
    console.log("Usage 1: leaf (execute Repl);")
    console.log("Usage 2: leaf <filePath> (execute a file);")
}

const runRepl = async () => {
    while (true) {
        console.log()
        const source = await utils.readInput("leaf > ")
        if (source === "exit") {
            console.log("Exiting Repl")
            return
        }
        run(source)
    }
}

const runFile = async (filePath: string) => {
    const source = await fs.readFile(filePath, "utf-8")
    run(source)
}

const run = (source: string) => {
    const tokens = Tokenizer.createInstance().getTokens(source)

    if (LeafError.getInstance().hasErrors()) {
        printErrors(LeafError.getInstance().errors())
        exit(1)
    }

    const stmts = Parser.createInstance().getStmts(tokens)

    if (LeafError.getInstance().hasErrors()) {
        printErrors(LeafError.getInstance().errors())
        exit(1)
    }

    Interpreter.createInstance().execute(stmts)
}

const printErrors = (errors: string[]) => {
    errors.forEach((error) => {
        console.error(error)
    })
}

const printTokens = (tokens: Token[]) => {
    tokens.forEach((token) => {
        console.log(token.toString())
    })
}

const exit = (status: number) => {
    process.exit(status)
}

await main(argc, argv)
exit(0)
