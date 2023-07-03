import {
    Alert,
    AlertTitle
}from "@mui/material/";

// Connection error 
function ConnectionAlert() {
    return (
        <div className="alert">
            <Alert severity="error">
                <AlertTitle>Server error</AlertTitle>
                Connection error — <strong>check the RPi</strong>
            </Alert>
        </div>
    )
}

export default ConnectionAlert