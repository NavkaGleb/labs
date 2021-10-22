window.addEventListener('load', () => {
	let controls     = document.getElementById('controls');
	let currentSlide = 1;

	setInterval(() => {
		controls.children[currentSlide++].click();
		currentSlide %= 3;
	}, 4000);
});