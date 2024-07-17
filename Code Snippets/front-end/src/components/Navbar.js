import React from "react";
import "./css/main.css";

const User = ({isLoggedIn}) => {
    return (
        <div>
            {isLoggedIn ? (
                <div className="navbar">
                    <button className="btn btn-success" onClick={() => {window.location.href = "/create"}} style={{marginRight: "30px"}}>Create</button>
                    <a type="button" className="d-flex profile btn" href="/profile" style={{marginRight: "20px"}}>{isLoggedIn.name}</a>
                </div>
            ) : (
                <div>
                    <a href="/login" className="login link badge text-bg-primary">Login</a>
                    <a href="/signup" className="signup link badge text-bg-success">Sign Up</a>
                </div>
            )}
        </div>
    );
}

const Navbar = ({isLoggedIn}) => {
    return (
        <nav className="navbar navbar-expand-lg bg-body-tertiary">
            <div className="container-fluid">
                <a className="navbar-brand" href="/" style={{fontWeight: "bold"}}>Code Snippets Manager</a>
                {<User isLoggedIn={isLoggedIn} />}
            </div>
        </nav>
    );
};

export default Navbar;