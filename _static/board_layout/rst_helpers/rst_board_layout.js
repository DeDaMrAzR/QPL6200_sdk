// this file is used by .rst pages in raw html block to modify the appearance of Board Layout SVG graphic

// Selects out Inkscape layer labels and modifies their visibility
// This works on a default <object> SVG element. For a different approach, see 'toggleLayer' and 'showAndHide' functions in js file for board_layout.html
function modifySVG(labelsToShow, labelsToHide){
    document.getElementById('svgObject').addEventListener('load', function() {
        const svgDoc = this.contentDocument;
        const layers = svgDoc.getElementsByTagName('g');
        for (let i = 0; i < layers.length; i++) {
            const label = layers[i].getAttribute('inkscape:label');
            if (labelsToShow.includes(label)) {
                layers[i].style.display = 'inline';
            }
             if (labelsToHide.includes(label)) {
                layers[i].style.display = 'none';
            }
       }
    })
};

function newWindowUrl(labelsToShow, labelsToHide, newWindowTitle){
        // Construct a link to open the Board Layout in new window with visibility setting matching the document
        // Get the base URL from the href attribute so we don't have to type it twice
        const baseUrl = document.getElementById('board-layout-new-window').href;
        // Construct the URL with parameters to open the Board Layout in new window
        const boardLayoutUrl = `${baseUrl}?toshow=${labelsToShow.join(',')}&tohide=${labelsToHide.join(',')}&pagetitle=${newWindowTitle}`;
        console.log(boardLayoutUrl)
        // Add the link to target element
        document.getElementById('board-layout-new-window').href = boardLayoutUrl
}
