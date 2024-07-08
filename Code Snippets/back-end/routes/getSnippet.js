const express = require("express");
const route = express.Router();
const { query, validationResult } = require("express-validator");

route.get(
    "/",
    [
        query("id").notEmpty().withMessage("Id is required")
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

        const { id } = req.query;

        try {
            const query = "SELECT * FROM snippets WHERE id = ?";
            req.db.query(query, [id], (err, results) => {
                if(err) {
                    console.error("Error:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }

                if(results.length === 0) {
                    return res.status(400).json({ error: "No such snippet" });
                }

                res.status(200).json({ snippet: results[0] });
            });
        } catch(err) {
            console.error("Error:", err);
            return res.status(500).json({ error: "Internal server error" });
        }
    }
)

module.exports = route;