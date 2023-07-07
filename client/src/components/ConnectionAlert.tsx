import {
    Alert,
    AlertTitle
}from "@mui/material/";

interface Error {
    title: string,
    info: string
}

// Connection error 
function ConnectionAlert({ title, info }: Error) {
    return (
        <div className="alert">
            <Alert severity="error">
                <AlertTitle>{title}</AlertTitle>
                <strong>{info}</strong>
            </Alert>
        </div>
    )
}

export default ConnectionAlert