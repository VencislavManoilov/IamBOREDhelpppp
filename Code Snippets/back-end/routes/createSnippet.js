const express = require("express");
const route = express.Router();
const { body, validationResult } = require("express-validator");
const isAuthenticated = require("../middleware/isAuthenticated");

route.post(
    "/",
    isAuthenticated,
    [
        body("title").notEmpty().withMessage("Title required"),
        body("code").notEmpty().withMessage("Code required"),
        body("type").notEmpty().withMessage("Type required")
    ],
    (req, res) => {
        // Check for validation errors
        const errors = validationResult(req);
        if(!errors.isEmpty()) {
            let errorMessages = [];
            for(let i = 0; i < errors.array().length; i++) {
                errorMessages.push(errors.array()[i].msg);
            }
            return res.status(400).json({ errors: errorMessages });
        }

        const { title, code, type } = req.body;

        if(!CheckType(type)) {
            return res.status(400).json({ error: "Type not supported" });
        }

        try {
            const query = `INSERT INTO snippets (user_id, title, code, type) VALUES (?, ?, ?, ?)`;
            const values = [req.session.userId, title, code, type];

            req.db.query(query, values, (err, result) => {
                if (err) {
                    console.error("Error saving snippet to database:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }

                return res.status(201).json({ id: result.insertId });
            });
        } catch (err) {
            console.error(err);
            return res.status(500).json({ error: "Internal server error" });
        }
    }
)

function CheckType(type) {
    const validTypes = new Set([
        'js', 'cpp', 'cs', 'java', 'py', 'rb', 'php', 'swift', 'go', 'rs', 
        'kt', 'ts', 'scala', 'dart', 'lua', 'perl', 'asm', 'sh', 'r', 'm',
        'vb', 'pl', 'clj', 'elixir', 'erlang', 'fsharp', 'groovy', 'haskell',
        'julia', 'lisp', 'objc', 'pascal', 'prolog', 'rust', 'sql', 'vhdl',
        'verilog', 'cobol', 'fortran', 'ada', 'tcl', 'awk', 'bash', 'zsh',
        'matlab', 'sas', 'sml', 'scheme', 'forth', 'ml', 'ocaml', 'nim', 
        'crystal', 'racket', 'red', 'rexx', 'vbnet', 'd', 'elm', 'idris',
        'coffeescript', 'postscript', 'io', 'smalltalk', 'abc', 'algol', 
        'apl', 'awk', 'bc', 'bliss', 'ch', 'csh', 'dcl', 'eiffel', 'forth',
        'icon', 'idl', 'j', 'kotlin', 'logo', 'modula2', 'nemerle', 'occam', 
        'opencl', 'plsql', 'pike', 'rex', 'rpg', 'simula', 'snobol', 'sparc',
        'spiral', 'sqf', 'stata', 'supercollider', 'systemverilog', 'vba',
        'vimscript', 'wren', 'xojo', 'yacc', 'zpl', 'actionscript', 'ampl',
        'antlr', 'awk', 'bcpl', 'boo', 'caml', 'clarion', 'clojure', 'cython',
        'dylan', 'ecmascript', 'esolang', 'factor', 'fscript', 'gams', 'gap',
        'gml', 'haxe', 'io', 'jacl', 'janet', 'kornshell', 'livecode', 'maple',
        'max', 'mercury', 'mumps', 'newlisp', 'nusmv', 'opal', 'picat', 'povray',
        'promela', 'puppet', 'pure', 'purescript', 'q', 'quil', 'rebol', 'sed',
        'smlnj', 'spark', 'turing', 'vala', 'verilog', 'vhdl', 'whiley', 'x10',
        'xtend', 'yacas', 'yorick', 'zimbu', 'zig'
    ]);
    return validTypes.has(type);
}

module.exports = route;