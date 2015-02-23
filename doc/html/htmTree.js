/* switch branches
*/

//<script language="JavaScript">
var imgc = new Image(); imgc.src = "bc.png";
var imgo = new Image(); imgo.src = "bo.png";

function brSwap(brId){
  var oi = document.getElementById("I"+brId);
  var ob = document.getElementById("B"+brId).style;
  if(ob.display=="block") {
    ob.display="none";
    oi.src = imgc.src;
  } else {
    ob.display="block";
    oi.src = imgo.src;
  }
}

//</script>
