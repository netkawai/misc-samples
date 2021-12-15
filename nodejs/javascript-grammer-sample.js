
// an example of something you probably shouldn't do. Ever. Seriously.
Number.prototype.addto = function(x){
    return this + x;
}

var result = (8).addto(9); // returns 17

console.log(result);
// other variations:


result = 10['addto'](9);  // works but is kind of ugly compared to the first method
console.log(result);

var eight = 8;
result = eight.addto(6);  // works

console.log(result);
