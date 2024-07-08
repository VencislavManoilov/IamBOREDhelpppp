const express = require("express");
const crypto = require("crypto");
const { body, validationResult } = require("express-validator");
const router = express.Router();

router.post("/",
    [
        // Validate and sanitize inputs
        body("name").notEmpty().withMessage("Name is required").trim().escape(),
        body("email").isEmail().withMessage("Invalid email format").normalizeEmail(),
        body("password")
        .isLength({ min: 6 })
        .withMessage("Password must be at least 6 characters long"),
        body("age").isInt({ min: 0 }).withMessage("Age must be a positive integer")
    ],

    async (req, res) => {
        // Check for validation errors
        const errors = validationResult(req);
        if(!errors.isEmpty()) {
            let errorMessages = [];
            for(let i = 0; i < errors.array().length; i++) {
                errorMessages.push(errors.array()[i].msg);
            }
            return res.status(400).json({ errors: errorMessages });
        }

        // Destructure the validated and sanitized input data
        const { name, email, password, age } = req.body;

        try {
            // Encrypt the password
            const hashedPassword = crypto.createHash('sha256').update(password).digest('hex').slice(0, 32);

            const checkEmailQuery = "SELECT * FROM users WHERE email = ?";
            req.db.query(checkEmailQuery, [email], (err, results) => {
                if(err) {
                    console.log("Error checking email:", err);
                    return res.status(500).json({ error: "Internal server error" });
                }

                if(results.length > 0) {
                    // Email is already used
                    return res.status(400).json({ error: "Email is already in use" });
                }

                // Insert user data into the database
                const query = "INSERT INTO users (name, email, password, age) VALUES (?, ?, ?, ?)";
                const values = [name, email, hashedPassword, age];
    
                req.db.query(query, values, (err, results) => {
                    if (err) {
                        console.error("Error saving user to the database:", err);
                        return res.status(500).json({ error: "Internal server error" });
                    }
                    res.status(201).json({ success: "User registered successfully" });
                });
            });

        } catch (error) {
            console.error(error);
            res.status(500).json({ error: "Internal server error" });
        }
    }
);

module.exports = router;