const input = document.querySelector("input");
const preview = document.querySelector(".preview");
const itemsList = document.getElementById("items-hook");
const submitBtn = document.querySelector("#submit-btn");

let urlUpload = "http://localhost:8080/website/upload-file";
let url = "";

let submitedFiles = [];

// Handle the front when a file is selected, preview.
const updateImageDisplay = () => {
	// remove image from preview div if have any
	while (preview.firstChild) {
		preview.removeChild(preview.firstChild);
	}
	const curFiles = input.files;
	// display text if there is no files in preview
	if (curFiles.length === 0) {
		const para = document.createElement("p");
		para.textContent = "No files currently selected for upload";
		preview.appendChild(para);
	} else {
		// display the image preview
		const list = document.createElement("ul");
		list.style.listStyleType = "none";
		preview.appendChild(list);
		for (const file of curFiles) {
			const listItem = document.createElement("li");
			if (validFileType(file)) {
				const image = document.createElement("img");
				image.style.width = "250px";
				image.style.height = "150px";
				image.src = URL.createObjectURL(file);
				listItem.appendChild(image);
			} else {
				para.textContent = `File name ${ file.name }: Not a valid file type. Update your selection.`;
				listItem.appendChild(para);
			}
			list.appendChild(listItem);
		}
	}
};

const validFileType = (file) => {
	return fileTypes.includes(file.type);
};

// list of accepted image
const fileTypes = ["image/gif", "image/jpeg", "image/png"];

const renderImages = () => {
	while (itemsList.firstChild) {
		itemsList.removeChild(itemsList.firstChild);
	}
	if (submitedFiles.length === 0) return;
	for (const images of submitedFiles) {
		itemsList.appendChild(images);
	}
	while (preview.firstChild) {
		preview.removeChild(preview.firstChild);
	}
	input.value = null;
};

const createIdNumber = () => {
	return Math.random().toString();
};

// Function that is called when we click on a existing file to delete it
const deleteExistingFile = (e) => {
	e.preventDefault();
	submitedFiles = submitedFiles.filter(
		(el) => el.id !== e.target.parentElement.id
	);
	/**
	 * TODO => send http DELETE methode to delete the file.
	 */
	renderImages();
};

const submitHandler = (e) => {
	e.preventDefault();
	/**
	 * * Frontend part
	 */
	const curFiles = input.files;
	if (curFiles.length === 0) return;
	const li = document.createElement("li");
	li.setAttribute("id", createIdNumber());
	const image = document.createElement("img");
	image.style.width = "250px";
	image.style.height = "150px";
	image.src = URL.createObjectURL(curFiles[0]);
	li.appendChild(image);
	li.addEventListener("click", deleteExistingFile);
	submitedFiles.push(li);
	renderImages();

	/**
	 * * Backend part
	 * * send http POST methode to send the file to the server.
	 * ! IT works but the server doesn't handle multipart/form-data for now.
	 */
	const files = curFiles;
	const data = new FormData();
	data.append("file", files[0]);
	data.append("id", createIdNumber());
	fetch(urlUpload, { method: "POST", mode: "no-cors", body: data }).then((response) => response.json()).then((data) => {
		console.log(data);
	}).catch((error) => {
		console.error(error);
	});
	console.log(data);
};

input.addEventListener("change", updateImageDisplay);
submitBtn.addEventListener("click", submitHandler);
