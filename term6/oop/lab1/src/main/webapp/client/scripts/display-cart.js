window.onload = async () => {
    document.getElementById('place-order').onclick = async function () {
        await fetch(`/servlets/cart/remove-all-items`);
        window.location.reload();
        alert("Order was successfully placed.");
    }

    document.getElementById('logout').onclick = async function () {
        await fetch(`/servlets/logout`, {
            method: 'POST'
        });
        window.location = "login.jsp"
    }
    await populateTable();
}

const populateTable = async () => {
    const table = document.getElementById('product-cart');
    const res = await fetch('/servlets/cart');
    const requests = await res.json();

    const createRow =
        (cartId, nameText, priceText, quantityText) => {
            const row = document.createElement('tr');

            const name = document.createElement('td');
            const price = document.createElement('td');
            const quantity = document.createElement('td');
            const actions = document.createElement('td');

            name.innerText = nameText;
            price.innerText = priceText;
            quantity.innerText = quantityText;

            let removeFromCart = document.createElement("button");
            removeFromCart.innerText = "remove"
            removeFromCart.className = "badge badge-danger";
            removeFromCart.style.marginRight = "10px";
            removeFromCart.onclick = async function () {
                await fetch(`/servlets/cart/remove-from-cart?id=${cartId}`);
                window.location.reload();
            }

            actions.appendChild(removeFromCart);

            row.appendChild(name);
            row.appendChild(quantity);
            row.appendChild(price);
            row.appendChild(actions);

            return row;
        };

    var fullPrice = 0;

    for (let request of requests) {
        fullPrice += request.price;
        const row = createRow(request.shoppingCartId, request.productName, request.price, request.quantity);
        table.appendChild(row);
    }

    document.getElementById('total-price').innerText = "Total price = " + fullPrice;
}