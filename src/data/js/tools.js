function VerseSelection() {
    this.getSelection = verseGetSelection;
    this.startVerse = -1;
    this.endVerse = -1;
    this.selectedText = "";
    this.moduleID = -1;
    this.bookID = -1;
    this.chapterID = -1;
    this.startChapterID = -1;
    this.endChapterID = -1;

}
function verseGetSelection () {
    console.log("getting verseselection");
    console.log(window.getSelection().type);
    /*if(window.getSelection().type == 'Range') {*/
        console.log("its a range");
        var a = window.getSelection().getRangeAt(0);
        if(a != null) {
            console.log("and range 0 is not null");
            this.selectedText = window.getSelection().toString();

            var start = a.startContainer;
            var end = a.endContainer;
            var it = start;
            //todo: make it better
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    this.startVerse = e.getAttribute("verseID");
                    this.startChapterID = e.getAttribute("chapterID");
                    break;
                } else {
                    it = e;
                }
            }
       
            console.log("we found start verse", this.startVerse);
            it = end;
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    this.endVerse = e.getAttribute("verseID");
                    this.moduleID = e.getAttribute("moduleID");
                    this.bookID = e.getAttribute("bookID");
                    this.endChapterID = e.getAttribute("chapterID");
                    break;
                } else {
                    it = e;
                }
            }

        }

	/*}*/
}


function AdVerseSelection() {
    this.getSelection = adVerseGetSelection;
    this.startVerse = -1;
}
function adVerseGetSelection () {
    if(window.getSelection().type == 'Range') {
        var a = window.getSelection().getRangeAt(0);
        if(a != null) {
            var node = document.createElement('span');
            node.appendChild(document.createTextNode("!-_OPENBIBLEVIEWER_INSERT_-!"));
            node.id = 'OBV_INSERT';
            this.selectedText = window.getSelection().toString();
            a.insertNode(node);
            var start = a.startContainer;
            var end = a.endContainer;
            var it = start;
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    this.startVerse = e.getAttribute("verseID");
                    this.startVerseText = e.textContent;
                    this.startVerseContent = e.innerHTML;
                    break;
                } else {
                    it = e;
                }
            }

            var x = document.getElementById("OBV_INSERT");
            x.parentNode.removeChild(x);
        }
    }
}
