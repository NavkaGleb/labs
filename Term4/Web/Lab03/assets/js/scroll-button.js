window.addEventListener("load", () => {
    let scrollButton = document.getElementById("scroll-button");

    scrollButton.addEventListener("click", () => {
        window.scrollTo(0, 0);
    });
});