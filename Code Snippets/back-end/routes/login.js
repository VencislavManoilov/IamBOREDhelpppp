const express = require("express");
const crypto = require("crypto");
const { body, validationResult } = require("express-validator");
const router = express.Router();

router.post(
    "/",
    [
        body("email").isEmail().withMessage("Invalid email format").normalizeEmail(),
        body("password").notEmpty().withMessage("Password is required")
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


        const { email, password } = req.body;

        try {
            // Encrypt the password to compare with the stored hash
            const hashedPassword = crypto.createHash('sha256').update(password).digest('hex').slice(0, 32);
    
            // Query to find the user by email and hashed password
            const query = "SELECT * FROM users WHERE email = ? AND password = ?";
            req.db.query(query, [email, hashedPassword], (err, results) => {
                if (err) {
                    console.error("Error querying the database:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }
    
                if (results.length === 0) {
                    return res.status(401).json({ error: "Invalid email or password" });
                }
    
                // Set the session
                req.session.userId = results[0].id;
                res.status(200).json({ success: "Login successful" });
            });
        } catch (error) {
            console.log("Error logging in", error);
            res.status(500).json({ error: "Internal server error" });
        }
    }
);

module.exports = router;