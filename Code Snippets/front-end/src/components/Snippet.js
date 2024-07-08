import React, { useEffect, useState } from "react";
import axios from "axios";
import "./css/main.css";
import hljs from 'highlight.js';
import 'highlight.js/styles/monokai-sublime.css';

const URL = process.env.REACT_APP_BACKEND_URL || process.env.REACT_APP_CUSTOM_BACKEND_URL || "http://localhost:8080";

const Snippet = ({ snippetId, hasBackButton, backToProfileFunction }) => {
    const [snippet, setSnippet] = useState(null);

    useEffect(() => {
        const fetchSnippet = async () => {
            try {
                const response = await axios.get(URL+"/snippet/get?id=" + snippetId);
                setSnippet(response.data.snippet);
            } catch (error) {
                console.log("Error getting the snippet:", error);
            }
        };

        fetchSnippet();
    }, [snippetId]);

    useEffect(() => {
        hljs.highlightAll();
    }, [snippet]);

    return (
        <div className="container mt-5">
            {hasBackButton ? (
                <button className="btn btn-secondary btn-block" type="button" onClick={backToProfileFunction}>Go to Profile</button>
            ) : (<></>)}
            {snippet ? (
                <div className="mt-3">
                    <h1>{snippet.title}</h1>
                    <pre><code className={"language-"+snippet.type}>{snippet.code}</code></pre>
                    <span className="h4">Type: <span className="badge text-bg-secondary">{snippet.type}</span></span>
                </div>
            ) : (
                <p>Loading snippet...</p>
            )}
        </div>
    );
};

export default Snippet;