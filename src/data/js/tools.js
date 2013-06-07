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
    this.getSelect = adVerseGetSelection;
    this.startVerse = -1;
    this.startVerseText = "";
    this.startVerseContent = "";
    this.selectedText = "not exec";
}
function adVerseGetSelection () {
    removeSelectionStuff();
    this.selectedText = "not a range"
    if(window.getSelection().type == 'Range') {
        var a = window.getSelection().getRangeAt(0);
        this.selectedText = "a null";
        if(a != null) {
            this.selectedText = "not null";
            var node = document.createElement('span');
            node.appendChild(document.createTextNode("!-_OPENBIBLEVIEWER_INSERT_-!"));
            node.id = 'OBV_INSERT';
            a.insertNode(node);
            
            this.selectedText = window.getSelection().toString();
            
            
            var start = a.startContainer;
            var end = a.endContainer;
            var it = start;
            //we are going out
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                //if we are at the verse level then save suff
                if(e.getAttribute("verseID") != null) {
                    this.startVerse = e.getAttribute("verseID");
                    this.startVerseText = e.textContent;
                    this.startVerseContent = e.innerHTML;
                    break;
                } else {
                    it = e;
                }
            }
            //todo: it makes the selection wrong (visual bug)
            removeSelectionStuff();
        }
    }
}
function removeSelectionStuff() {
    var x = document.getElementById("OBV_INSERT");
    if(x != null) {
        x.parentNode.removeChild(x);
    }
}
function ReplaceContentInContainer(selector, content) {
  var nodeList = document.querySelectorAll(selector);
  for (var i = 0, length = nodeList.length; i < length; i++) {
     nodeList[i].innerHTML = content;
  }
}