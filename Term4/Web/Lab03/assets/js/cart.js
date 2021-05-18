window.addEventListener("load", () => {
    let totalPrice = 0;

    let cartText   = document.getElementById("cart").getElementsByTagName("span")[0];
    let buttons    = document.getElementsByClassName("btn btn-sm btn-danger w-100 mt-1");
    let prices     = document.getElementsByClassName("catalog-item-price");

    console.log(prices);

    for (let i = 0; i < buttons.length; ++i) {
        buttons[i].addEventListener("click", () => {
            totalPrice        += parseFloat(prices[i].innerHTML);
            cartText.innerHTML = totalPrice.toString();
        });
    }
});