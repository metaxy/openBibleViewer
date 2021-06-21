function simpleVerseGetSelection () {
    console.log("getting verseselection");
    console.log(window.getSelection().type);
    /*if(window.getSelection().type == 'Range') {*/
        var a = window.getSelection().getRangeAt(0);
        if(a != null) {
            console.log("and range 0 is not null");
            VerseSelection.selectedText = window.getSelection().toString();

            var start = a.startContainer;
            var end = a.endContainer;
            var it = start;
            //todo: make it better
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    VerseSelection.startVerse = e.getAttribute("verseID");
                    VerseSelection.startChapterID = e.getAttribute("chapterID");
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
                    VerseSelection.endVerse = e.getAttribute("verseID");
                    VerseSelection.moduleID = e.getAttribute("moduleID");
                    VerseSelection.bookID = e.getAttribute("bookID");
                    VerseSelection.endChapterID = e.getAttribute("chapterID");
                    break;
                } else {
                    it = e;
                }
            }

        }

	/*}*/
}

function adVerseGetSelection () {
    removeSelectionStuff();
    VerseSelection.selectedText = "not a range"
    if(window.getSelection().type == 'Range') {
        var a = window.getSelection().getRangeAt(0);
        VerseSelection.selectedText = "a null";
        if(a != null) {
            VerseSelection.selectedText = "not null";
            var node = document.createElement('span');
            node.appendChild(document.createTextNode("!-_OPENBIBLEVIEWER_INSERT_-!"));
            node.id = 'OBV_INSERT';
            a.insertNode(node);
            
            VerseSelection.selectedText = window.getSelection().toString();
            
            
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
                    VerseSelection.startVerse = e.getAttribute("verseID");
                    VerseSelection.startVerseText = e.textContent;
                    VerseSelection.startVerseContent = e.innerHTML;
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
function replaceContentInContainer(selector, content) {
  var nodeList = document.querySelectorAll(selector);
  for (var i = 0, length = nodeList.length; i < length; i++) {
     nodeList[i].innerHTML = content;
  }
}