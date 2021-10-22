window.addEventListener('load', () => {
    document.getElementById('header-feedback-button').onclick     = openFeedbackForm;
   	document.getElementById('feedback-form-close-button').onclick = closeFeedbackForm; 
});

function openFeedbackForm() {
	document.body.style.overflow = 'hidden';

	let feedbackBackgroundContainer                 = document.getElementById('feedback-background-container');
	feedbackBackgroundContainer.style.animationName = 'open-form';
	feedbackBackgroundContainer.style.display       = 'flex';
}

function closeFeedbackForm() {
	document.body.style.overflow                                                 = 'visible';
	document.getElementById('feedback-background-container').style.animationName = 'close-form';
}