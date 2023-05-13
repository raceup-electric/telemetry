import Alert from "@mui/material/Alert";
import AlertTitle from "@mui/material/AlertTitle";

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