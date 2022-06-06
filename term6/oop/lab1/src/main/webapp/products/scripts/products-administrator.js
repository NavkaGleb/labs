window.onload = async () => {
    document.getElementById('logout').onclick = async function () {
        await fetch(`/servlets/logout`, {
            method: 'POST'
        });
        window.location = "login.jsp"
    }

    await populateTable();
}

const populateTable = async () => {
    const table = document.getElementById('products');
    const res = await fetch('/servlets/products');
    const requests = await res.json();

    const createRow = (nameText, priceText, descriptionText, productTypeText) => {
        const row = document.createElement('tr');

        const name = document.createElement('td');
        const price = document.createElement('td');
        const description = document.createElement('td');
        const productType = document.createElement('td');

        name.innerText = nameText;
        price.innerText = priceText;
        description.innerText = descriptionText;
        productType.innerText = productTypeText;

        row.appendChild(name);
        row.appendChild(price);
        row.appendChild(description);
        row.appendChild(productType);

        return row;
    };

    const changeButtons = (request) => {
        const actions = document.createElement('td');

        let editButton = document.createElement("button");
        editButton.innerText = "edit";
        editButton.className = "btn btn-primary";
        editButton.style.marginRight = "10px";
        editButton.onclick = function () {
            window.location = `http://localhost:8080/products/product-manager.jsp?id=${request.productId}`;
        }

        let removeButton = document.createElement("button");
        removeButton.innerText = "remove";
        removeButton.className = "btn btn-danger";
        removeButton.onclick = async function () {
            await fetch(`/servlets/products/remove?id=${request.productId}`);
            window.location.reload();
        }

        actions.appendChild(editButton);
        actions.appendChild(removeButton);

        return actions
    }

    for (let request of requests) {
        const row = createRow(request.name, request.price, request.description, request.productType);

        row.appendChild(changeButtons(request));
        table.appendChild(row);
    }
};