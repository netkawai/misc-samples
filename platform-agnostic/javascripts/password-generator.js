
// simple password generator in Javascript
var pass = ''; 
for(var i = 0 ; i < 8 ; i++){ 
    console.log(String.fromCharCode(Math.floor(Math.random()*74) + 48)); 
} 