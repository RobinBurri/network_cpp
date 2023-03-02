const form = document.getElementById('form');
const input = document.getElementById('text-input');
const itemsList = document.getElementById('items-hook');

const url = "http://localhost:8080/website/404.html";




const submitHandler = (e) => {
	e.preventDefault();
	if (input.value.length === 0) return;
	const htmlEl = createHtmlEl(input.value);
	itemsList.appendChild(htmlEl);
	sendHttpRequest('POST', url, input.value);
	input.value = "";
}

const createHtmlEl = (str) => {
	const div = document.createElement('div');
	div.className = 'item-div';
	const h2 = document.createElement('h2');
	h2.textContent = str;
	div.appendChild(h2);
	return div;
}


form.addEventListener('click', submitHandler);

const sendHttpRequest = (method, url, data) => {
	return fetch(url, { method: method, mode: "no-cors", body: data });
}

// Quand on click sur Submit button dans le website il envoit un POST request à http://localhost:8080/testpost
// avec pour body le text entré dans le input field. (si input field vide, il ne fait pas de request)
// Pour l'instant il n'y a que POST request qui est implemanté.