const checkFormInputs = async () => {
    document.getElementById('login-submit').disabled = true;

    const data = new Map([
        ['username', document.getElementById('username').value],
        ['password', document.getElementById('password').value],
    ]);

    for (let [key, value] of data) {
        if (value == null) {
            formFailure(`The ${key} input is missing. Please refresh the page and try again.'`);
            return;
        }
    }

    for (let [key, value] of data) {
        data.set(key, value.trim());

        if (data.get(key) === '') {
            formFailure(`The ${key} input is empty. Please fill it out and try again.`);
            return;
        }
    }

    const encodedData = [];

    data.forEach((value, key) => {
        encodedData.push(`${key}=${value}`);
    });

    const body = encodedData.join('&');

    const response = (await fetch('servlets/login', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: body
    }));

    const request = await response.json();

    if (request != null) {
        let reply = JSON.parse(JSON.stringify(request)).toLowerCase();

        if (reply === "blocked") {
            formFailure("The user is blocked.")
        } else if (reply === "notexists") {
            formFailure("Login details are incorrect.")
        } else {
            window.location.href = `${reply}/home.jsp`
        }
    } else {
        formFailure("Something went wrong")
    }
}

const formFailure = (errorMessage) => {
    const modalText = document.getElementById('failureModalText');
    modalText.innerText = errorMessage;
    $('#failureModal').modal(null);
    document.getElementById('login-submit').disabled = false;
}