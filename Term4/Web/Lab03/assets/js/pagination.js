const PAGE_ITEM_COUNT = 4;

function generatePage(text) {
    let page     = document.createElement("li");
    let pageLink = document.createElement("a");

    page.classList.add("page-item");

    pageLink.classList.add("page-link");
    pageLink.style.color = "#dc3545";
    pageLink.href = "#catalog-items";
    pageLink.innerText = text;

    page.appendChild(pageLink);

    return page;
}

window.addEventListener("load", () => {
    // create page-active .css class
    let activePage = document.createElement("style");
    activePage.innerHTML = ".active-page { " +
        "background: #dc3545 !important; " +
        "color: white !important; " +
    "}";

    document.getElementsByTagName("head")[0].appendChild(activePage);

    let pagination     = document.getElementById("pagination");
    let items          = document.getElementsByClassName("catalog-item");

    let pageCount      = Math.ceil(items.length / PAGE_ITEM_COUNT);
    let prevPageNumber = null;

    // generate pages
    for (let i = 0; i < pageCount + 2; ++i) {
        let text = i.toString();

        if (i === 0)
            text = "Previous";

        if (i === pageCount + 1)
            text = "Next";

        let page = generatePage(text);

        pagination.appendChild(page);

        page.addEventListener("click", (event) => {
            let innerText = event.currentTarget.innerText;

            if (innerText === "Previous") {
                if (prevPageNumber !== 1 && prevPageNumber !== null)
                    pagination.children[prevPageNumber - 1].click();

                return;
            }

            if (innerText === "Next") {
                if (prevPageNumber !== pageCount)
                    pagination.children[prevPageNumber + 1].click();

                return;
            }

            let currentPageNumber = parseInt(event.currentTarget.innerText);

            if (prevPageNumber !== null) {
                let prevPage = pagination.children[prevPageNumber];

                prevPage.children[0].classList.toggle("active-page");

                for (let i = PAGE_ITEM_COUNT * (prevPageNumber - 1); i < PAGE_ITEM_COUNT * prevPageNumber; ++i)
                    if (i < items.length)
                        items[i].classList.toggle("catalog-item-active");
            }

            prevPageNumber = currentPageNumber;

            event.currentTarget.children[0].classList.toggle("active-page");

            for (let i = PAGE_ITEM_COUNT * (currentPageNumber - 1); i < PAGE_ITEM_COUNT * currentPageNumber; ++i)
                if (i < items.length)
                    items[i].classList.toggle("catalog-item-active");
        });
    }

    // imitate click to first page
    pagination.children[1].click();
});