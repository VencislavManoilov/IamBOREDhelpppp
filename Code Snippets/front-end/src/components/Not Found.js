import React from "react";

const NotFound = () => {
    return (
        <div className="container text-center" style={{ marginTop: "100px" }}>
            <div className="row">
                <div className="col">
                    <h1 className="display-1">404</h1>
                    <h2 className="display-4">Page Not Found</h2>
                    <p className="lead">Sorry, the page you are looking for does not exist.</p>
                    <a type="button" className="btn btn-primary" href="/">Go to Homepage</a>
                </div>
            </div>
        </div>
    );
}

export default NotFound;