function toggleActive(element) {
    element.classList.toggle('active');
}
// toggles color on buttons in top menu
function toggleActive(element) {
    element.classList.toggle('active');
};
// Shows and hides Inkscape layer names. Used by buttons in top menu.
function toggleLayer(layerName) {
    // The selector below works only if the SVG is embedded directly in HTML. It fails to validate when the SVG is brought into DOM via script, even though an equivalent iterator works (see below)
    // This or an equivalent would be preferable than current solution. It would be more responsible computationally, because it doesn't iterate through a list of objects.
    // const layers = document.querySelectorAll(`[inkscape\\:label="${layerName}"]`);
    const layers = document.querySelectorAll(`g`);
    for (let i = 0; i < layers.length; i++) {
        const label = layers[i].getAttribute('inkscape:label');
        if (layerName == label) {
            if (layers[i].style.display === 'none'){
                layers[i].style.display = 'inline';
            }
            else {
                layers[i].style.display = 'none';
            }
        }
    }
};
// shows and hides elements passed in 2 lists. Intended use is displaying markings/tags on PCB, but it works with any element.
function showAndHide(labelsToShow, labelsToHide) {
    const layers = document.querySelectorAll(`g`);
    for (let i = 0; i < layers.length; i++) {
        const label = layers[i].getAttribute('inkscape:label');
        if (labelsToShow.includes(label)) {
            layers[i].style.display = 'inline'
        }
        if (labelsToHide.includes(label)) {
            layers[i].style.display = 'none'
        }
    }
}
// Customize the page title and menu title bar
function modifyWindowTitle(newWindowTitle) {
    // change document title
    var currentTitle = document.title;
    var newTitle = currentTitle + " - " + newWindowTitle;
    document.title = newTitle;
    // change title bar content
    var titleBar = document.getElementById("titlebar");
    titleBar.textContent = newTitle;
}
