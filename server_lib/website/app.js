const form = document.getElementById('form');
const input = document.getElementById('text-input');
const itemsList = document.getElementById('items-hook');



const submitHandler = (e) => {
	e.preventDefault();
	if (input.value.length === 0) return;
	console.log(input.value);
	const htmlEl = createHtmlEl(input.value);
	console.log(htmlEl);
	itemsList.appendChild(htmlEl);
	input.value = "";
}

const createHtmlEl = (str) => {
	const div = document.createElement('div');
	div.className = 'item-div';
	const h2 = document.createElement('h2');
	h2.textContent = str;
	div.appendChild(h2);
	console.log(h2);
	return div;
}


form.addEventListener('click', submitHandler);
