import React, { useState, useEffect, useRef } from "react";
import axios from "axios";
import Snippet from "./Snippet";

const URL = process.env.REACT_APP_BACKEND_URL || process.env.REACT_APP_CUSTOM_BACKEND_URL || "http://localhost:8080";

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

const Create = () => {
    const [title, setTitle] = useState("");
    const [code, setCode] = useState("");
    const [type, setType] = useState("js");
    const [view, setView] = useState("page");
    const [snippetId, setId] = useState();
    const textareaRef = useRef(null);

    useEffect(() => {
        const textarea = textareaRef.current;

        const handleTabPress = (event) => {
            if (event.key === 'Tab') {
                event.preventDefault();
                const start = textarea.selectionStart;
                const end = textarea.selectionEnd;

                // Set textarea value to: text before caret + tab + text after caret
                textarea.value = textarea.value.substring(0, start) + "\t" + textarea.value.substring(end);

                // Put caret at right position again
                textarea.selectionStart = textarea.selectionEnd = start + 1;

                // Adjust the height of the textarea
                updateTextarea();
            }
        };

        textarea.addEventListener('keydown', handleTabPress);

        return () => {
            textarea.removeEventListener('keydown', handleTabPress);
        };
    }, []);

    const updateTextarea = () => {
        const textarea = textareaRef.current;
        textarea.style.overflow = 'hidden';
        if(parseFloat(textarea.style.height) < 300) {
            textarea.style.height = "300px";
        } else {
            textarea.style.height = textarea.scrollHeight + "px";
        }
    };

    useEffect(() => {
        updateTextarea();
    }, [code]);

    const CheckType = (type) => {
        return validTypes.has(type);
    }

    const Send = async () => {
        if(title.trim() === "" || code.trim() === "" || !CheckType(type)) {
            document.getElementById("error").style.display = "block";
            return;
        }

        try {
            const response = await axios.post(URL+"/snippet/create", {
                title: title,
                code: code,
                type: type
            }, { withCredentials: true });

            setId(response.data.id);

            window.location.href = `/snippet?id=${response.data.id}`;
            setView("snippet");
        } catch(error) {
            document.getElementById("error").textContent = "Something went wrong ";
        }

        return (<></>);
    }

    return (
        <>
        {view === "page" ? (
            <div className='container mt-5' style={{backgroundColor: "rgba(0, 0, 0, 0.15)", border: "2px solid rgba(0, 0, 0, 0.3)", borderRadius: "20px"}}>
                <div className="container mt-5">
                    <div id="error" style={{display: 'none', position: 'sticky'}} className="alert alert-danger alert-dismissible fade show">
                        The fields are not filled!
                        <button type="button" className="btn-close" onClick={() => {document.getElementById("error").style.display = "none"}}></button>
                    </div>
                    <div className="col-lg-6">
                        <label htmlFor="title" style={{fontSize: 30, fontWeight: "bold"}}>Title</label>
                        <input
                            id="title"
                            className="form-control form-control-lg"
                            type="text"
                            value={title}
                            onChange={(e) => setTitle(e.target.value)}
                        />
                    </div>
        
                    <div className="col-12 mt-4">
                        <label htmlFor="textarea" className="form-label" style={{fontSize: 25, fontWeight: "bold"}}>Code</label>
                        <textarea
                            id="textarea"
                            className="form-control"
                            rows={10}
                            ref={textareaRef}
                            value={code}
                            onChange={(e) => setCode(e.target.value)}
                            onInput={updateTextarea}
                            style={{ overflow: 'hidden', resize: 'none' }}
                        />
                    </div>
        
                    <div className="col-8 col-lg-3 mt-3 row text-center">
                        <label htmlFor="select" className="form-label col-auto" style={{fontSize: 22, fontWeight: "bold"}}>Type</label>
                        <select
                            id="select"
                            className="form-control selectpicker col"
                            value={type}
                            onChange={(e) => setType(e.target.value)}
                        >
                            {[...validTypes].map((lang) => (
                                <option key={lang} value={lang}>
                                    {lang.toUpperCase()}
                                </option>
                            ))}
                        </select>
                    </div>
        
                    <div className="col-12 text-end" style={{marginBottom: '50px'}}>
                        <button className="btn btn-success" onClick={Send}>Send</button>
                    </div>
                </div>
            </div>
        ) : (
            <Snippet snippetId={snippetId} hasBackButton={true} backToProfileFunction={() => {window.location.href="/profile"}} />
        )
        }
        </>
    );
}

export default Create;