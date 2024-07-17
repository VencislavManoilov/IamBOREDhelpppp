import React, { useState, useEffect, useRef } from "react";
import { useLocation, useParams } from "react-router-dom";
import axios from "axios";

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

const Edit = () => {
    const { snippetId } = useParams();
    const location = useLocation();
    const { snippetContent } = location.state || {};

    const [title, setTitle] = useState("");
    const [code, setCode] = useState("");
    const [type, setType] = useState("");
    const [loading, setLoading] = useState(true);
    const textareaRef = useRef(null);
    
    useEffect(() => {
        if(snippetContent) {
            setTitle(snippetContent.title);
            setCode(snippetContent.code);
            setType(snippetContent.type);
            setLoading(false);
        } else {
            GetSnippet();
        }
    }, [snippetContent]);

    useEffect(() => {
        const textarea = textareaRef.current;

        if(!textarea) return;

        const handleTabPress = (event) => {
            if(event.key === 'Tab') {
                event.preventDefault();
                const start = textarea.selectionStart;
                const end = textarea.selectionEnd;

                // Set textarea value to: text before caret + tab + text after caret
                setCode(code.substring(0, start) + "\t" + code.substring(end));

                // Put caret at right position again
                setTimeout(() => {
                    textarea.selectionStart = textarea.selectionEnd = start + 1;
                }, 0);

                // Adjust the height of the textarea
                updateTextarea();
            }
        };

        textarea.addEventListener('keydown', handleTabPress);

        return () => {
            textarea.removeEventListener('keydown', handleTabPress);
        };
    }, [code]);

    useEffect(() => {
        const textarea = textareaRef.current;
        if(textarea) {
            textarea.style.height = 'auto'; // Reset the height
            textarea.style.height = `${textarea.scrollHeight}px`; // Set the new height
        }
    }, [code]);

    const updateTextarea = () => {
        const textarea = textareaRef.current;
        if(textarea) {
            textarea.style.overflow = 'hidden';
            textarea.style.height = 'auto'; // Reset the height
            textarea.style.height = `${textarea.scrollHeight}px`; // Set the new height
        }
    };

    const GetSnippet = async () => {
        try {
            const result = await axios.get(URL+"/snippet/get", {
                params: { id: snippetId }
            }, { withCredentials: true });

            if(result.data) {
                setTitle(result.data.title);
                setCode(result.data.code);
                setType(result.data.type);
                setLoading(false);
            }
        } catch (err) {
            console.log("Error: ", err);
        }
    }

    const CheckType = (type) => {
        return validTypes.has(type);
    }

    const Done = async () => {
        if(title.trim() === "" || code.trim() === "" || !CheckType(type)) {
            document.getElementById("error").style.display = "block";
            return;
        }

        try {
            await axios.put(URL+"/snippet/edit", {
                id: snippetId,
                title: title,
                code: code,
                type: type
            }, { withCredentials: true });

            window.location.href = "/profile";
        } catch (error) {
            document.getElementById("error").textContent = "Something went wrong ";
        }
    }

    return (
        <>
            {loading ? (
                <p>Loading...</p>
            ) : (
                <div className='container mt-5' style={{backgroundColor: "rgba(0, 0, 0, 0.15)", border: "2px solid rgba(0, 0, 0, 0.3)", borderRadius: "20px"}}>
                    <div className="container mt-3">
                        <div id="error" style={{ display: 'none', position: 'sticky' }} className="alert alert-danger alert-dismissible fade show">
                            The fields are not filled!
                            <button type="button" className="btn-close" onClick={() => { document.getElementById("error").style.display = "none" }}></button>
                        </div>

                        <div className="fs-1 fw-semibold text-light">Edit</div>

                        <div className="col-lg-6">
                            <label htmlFor="title" style={{ fontSize: 30, fontWeight: "bold" }}>Title</label>
                            <input
                                id="title"
                                className="form-control form-control-lg"
                                type="text"
                                value={title}
                                onChange={(e) => setTitle(e.target.value)}
                            />
                        </div>
            
                        <div className="col-12 mt-4">
                            <label htmlFor="textarea" className="form-label" style={{ fontSize: 25, fontWeight: "bold" }}>Code</label>
                            <textarea
                                id="textarea"
                                className="form-control"
                                rows={10}
                                ref={textareaRef}
                                value={code}
                                onChange={(e) => setCode(e.target.value)}
                                onInput={updateTextarea}
                            />
                        </div>
            
                        <div className="col-8 col-lg-3 mt-3 row text-center">
                            <label htmlFor="select" className="form-label col-auto" style={{ fontSize: 22, fontWeight: "bold" }}>Type</label>
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
            
                        <div className="col-12 text-end" style={{ marginBottom: '50px' }}>
                            <button className="btn btn-success" onClick={Done}>Done</button>
                        </div>
                    </div>
                </div>
            )}
        </>
    );
}

export default Edit;