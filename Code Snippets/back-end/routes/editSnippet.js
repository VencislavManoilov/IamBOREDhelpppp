const express = require("express");
const router = express.Router();
const { body, validationResult } = require("express-validator");
const isAuthenticated = require("../middleware/isAuthenticated");

router.put(
    "/",
    isAuthenticated,
    [
        body("id").notEmpty().withMessage("Id is required"),
        body("title").notEmpty().withMessage("Title is required"),
        body("code").notEmpty().withMessage("Code is required"),
        body("type").notEmpty().withMessage("Type is required")
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

        const { id, title, code, type } = req.body;

        try {
            const checkQuery = "SELECT * FROM snippets WHERE id = ?";
            req.db.query(checkQuery, [id], (err, results) => {
                if(err) {
                    console.error("Error:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }

                if(results.length === 0) {
                    return res.status(400).json({ error: "No such snippet" });
                }

                if(results[0].user_id != req.session.userId) {
                    return res.status(400).json({ error: "This snippet is not yours" });
                }

                const query = "UPDATE snippets SET title = ?, code = ?, type = ? WHERE id = ?";
                req.db.query(query, [title, code, type, id], (err, results) => {
                    if(err) {
                        console.error("Error:", err);
                        return res.status(500).json({ error: "Internal server error" });
                    }

                    return res.status(200).json({ success: true });
                })
            });
        } catch(err) {
            console.error("Error:", err);
            return res.status(500).json({ error: "Internal server error" });
        }
    }
)

module.exports = router;