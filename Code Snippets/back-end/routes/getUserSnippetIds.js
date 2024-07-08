const express = require("express");
const route = express.Router();
const { body, validationResult } = require("express-validator");
const isAuthenticated = require("../middleware/isAuthenticated");

route.get(
    '/',
    isAuthenticated,
    (req, res) => {
        try {
            const query = "SELECT * FROM snippets WHERE user_id = ?";
            req.db.query(query, [req.session.userId], (err, results) => {
                if(err) {
                    console.error("Error:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }

                res.status(200).json({ snippets: results.map(results => results.id) });
            });
        } catch(err) {
            console.error("Error:", err);
            return res.status(500).json({ error: "Internal server error" });
        }
    }
)

module.exports = route;