import React, { useEffect, useState } from "react";
import { useNavigate, useLocation, useParams } from "react-router-dom";
import axios from "axios";
import "./css/main.css";
import hljs from 'highlight.js';
import 'highlight.js/styles/monokai-sublime.css';
import QRCode from 'qrcode.react';

const URL = process.env.REACT_APP_BACKEND_URL || process.env.REACT_APP_CUSTOM_BACKEND_URL || "http://localhost:8080";

const Snippet = () => {
    // Get params and location state
    const { snippetId } = useParams();
    const location = useLocation();
    const { hasBackButton, theSnippet } = location.state || {};

    const [snippet, setSnippet] = useState(null);
    const [showQRCode, setShowQRCode] = useState(false);
    const [canEdit, setEdit] = useState(false);
    const [copyMessage, setCopyMessage] = useState("");
    
    useEffect(() => {
        if(!theSnippet) {
            const fetchSnippet = async () => {
                try {
                    const response = await axios.get(URL+"/snippet/get", { params: { id: snippetId } });
                    setSnippet(response.data.snippet);
                } catch (error) {
                    window.location.href = "/not_found";
                }
            };
            fetchSnippet();
        } else {
            setSnippet(theSnippet);
        }
    }, [snippetId]);

    useEffect(() => {
        hljs.highlightAll();
    }, [snippet]);

    useEffect(() => {
        const getUser = async () => {
            try {
                const response = await axios.get(URL+"/user", { withCredentials: true });
                if(response.status === 200) {
                    if(response.data.user.id === snippet.user_id) {
                        setEdit(true);
                    }
                }
            } catch (err) {}
        }

        if(snippet) {
            getUser();
        }
    }, [snippet])

    const navigate = useNavigate();
    const Edit = (snippetContent) => {
        navigate(`/edit/${snippetId}`, { state: { snippetContent } });
    }

    const CopyURL = () => {
        if(navigator.clipboard) {
            navigator.clipboard.writeText(window.location.origin + "/snippet/" + snippetId)
            .then(() => {
                setCopyMessage("Copied to clipboard");
            })
            .catch(err => {
                setCopyMessage("Couldn't be copied");
                console.error('Failed to copy:', err);
            });
        } else {
            setCopyMessage("Couldn't be copied");
        }
    }

    const handleShowQRCode = () => {
        setShowQRCode(true);
    };

    return (
        <div className="container mt-5">
            {hasBackButton ? (
                <button className="btn btn-secondary btn-block" type="button" onClick={() => {window.location.href = "/profile"}}>Go to Profile</button>
            ) : (<></>)}
            {snippet ? (
                <div className="mt-3">
                    <div className="d-flex justify-content-between align-items-center">
                        <h1 className="mb-0">{snippet.title}</h1>
                        {canEdit ? (<button type="button" className="btn btn-dark" onClick={() => {Edit(snippet)}}><svg className="feather feather-edit" fill="none" height="24" stroke="currentColor" strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" viewBox="0 0 24 24" width="24" xmlns="http://www.w3.org/2000/svg"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg></button>): (<></>)}
                    </div>
                    <pre><code className={"mt-2 language-"+snippet.type}>{snippet.code}</code></pre>
                    <span className="h4">Type: <span className="badge text-bg-secondary">{snippet.type}</span></span>

                    <div className="row col-12 mt-3">

                        <button type="button" className="btn btn-secondary col-auto me-3" data-bs-toggle="modal" data-bs-target="#exampleModal2" onClick={handleShowQRCode}>Show QR code</button>

                        <div className="modal fade" id="exampleModal2" tabIndex="-1" aria-labelledby="exampleModalLabel2" aria-hidden="true">
                            <div className="modal-dialog">
                                <div className="modal-content">
                                    <div className="modal-header">
                                        <h1 className="modal-title fs-5" id="exampleModalLabel2">QR Code</h1>
                                        <button type="button" className="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                                    </div>
                                    <div className="modal-body text-center" style={{margin: "none"}}>
                                        {showQRCode && <QRCode value={window.location.origin+"/snippet/" + snippetId} size={256} />}
                                    </div>
                                    <div className="modal-footer">
                                        <button type="button" data-bs-dismiss="modal" className="btn btn-primary">OK</button>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <button type="button" className="btn btn-primary col-auto" onClick={() => {CopyURL()}} data-bs-toggle="modal" data-bs-target="#exampleModal">
                        Copy URL
                        </button>

                        <div className="modal fade" id="exampleModal" tabIndex="-1" aria-labelledby="exampleModalLabel" aria-hidden="true">
                            <div className="modal-dialog">
                                <div className="modal-content">
                                    <div className="modal-header">
                                        <h1 className="modal-title fs-5" id="exampleModalLabel">{copyMessage}</h1>
                                    </div>
                                    <div className="modal-footer">
                                        <button type="button" data-bs-dismiss="modal" className="btn btn-primary">OK</button>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            ) : (
                <p>Loading snippet...</p>
            )}
        </div>
    );
};

export default Snippet;