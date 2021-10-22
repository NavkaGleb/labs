let pages;
let currentPage;
let paginatorItems = [];

function fillPagesGaps() {
    let holderWidth = parseInt(getComputedStyle(pagesHolder).width);
    let cardWidth = 250;
    let rowSize = Math.trunc(holderWidth / cardWidth);

    for (let page of pages) {
        let cardsCount = page.childElementCount;
        let gapsCount = (rowSize - (cardsCount % rowSize)) % rowSize;

        for (let i = 0; i < gapsCount; i++) {
            let gap = document.createElement('div');
            gap.classList.add('product-card');
            gap.classList.add('product-card-gap');
            gap.style.opacity = '0';
            page.append(gap);
        }
    }
}

function hidePage(page) {
    page.classList.remove('catalog-page-active');
}

function showPage(page) {
    page.classList.add('catalog-page-active');
}

function navItemClickHandle(e) {
    let prevIndex = currentPage;
    let index     = paginatorItems.findIndex((item) => item === e.target);

    if (prevIndex === index) 
    	return;

    currentPage = index;

    paginatorItems[prevIndex].classList.remove('paginator-item-active');
    paginatorItems[index].classList.add('paginator-item-active');

    hidePage(pages[prevIndex]);
    showPage(pages[index]);

    if (document.getElementById('content').offsetTop < window.pageYOffset)
        pages[index].scrollIntoView();
}

function initiateCatalogue() {
    pagesHolder = document.getElementById('catalog-pages-container');
    pages       = document.getElementsByClassName('catalog-page');
    currentPage = 0;

    let paginatorContainer = document.getElementById('paginator');

    for (let i = 0; i < pages.length; ++i) {
        let paginatorItem = document.createElement('div');

        paginatorItem.classList.add('paginator-item');
        paginatorItem.innerText = `${i + 1}`;

        paginatorItem.onclick = navItemClickHandle;

        paginatorContainer.append(paginatorItem);
        paginatorItems.push(paginatorItem);
    }

	paginatorItems[currentPage].classList.add('paginator-item-active');

    showPage(pages[currentPage]);
}

window.addEventListener('load', initiateCatalogue);