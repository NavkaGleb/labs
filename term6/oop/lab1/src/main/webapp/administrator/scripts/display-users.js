window.onload = async () => {
    document.getElementById('logout').onclick = async function () {
        await fetch(`/servlets/logout`, {
            method: 'POST'
        });
        window.location = "http://localhost:8080/login.jsp"
    }

    await populateTable();
}

const populateTable = async () => {
    const table = document.getElementById('users');
    const res = await fetch('/servlets/users');
    const requests = await res.json();

    const createRow = (id, nameText, blocked) => {
        const row = document.createElement('tr');
        const name = document.createElement('td');
        const actions = document.createElement('td');

        name.innerText = nameText;

        let blockButton = document.createElement("button");

        if (!blocked) {
            blockButton.innerText = "Block"
            blockButton.className = "btn btn-danger";
            blockButton.onclick = async function () {
                await fetch(`/servlets/users/block?id=${id}`)
                window.location.reload()
            }
        } else {
            blockButton.innerText = "Unblock"
            blockButton.className = "btn btn-success";
            blockButton.onclick = async function () {
                await fetch(`/servlets/users/unblock?id=${id}`)
                window.location.reload()
            }
        }

        actions.appendChild(blockButton)
        row.appendChild(name)
        row.appendChild(actions)

        return row;
    };

    for (let request of requests) {
        table.appendChild(createRow(request.id, request.name, request.blocked));
    }
}
