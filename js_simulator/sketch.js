
var angle = 0.0;
var freq = 0.01;
var colorOffset = 1.5;
var ballOffset = 1.5;
var midX = 0;
var midY = 0;
var numBalls = 36;
var theColumn = null;



function setup () {
  createCanvas(2000,2000);
  stroke(0);
  strokeWeight(1);
  midX = width/2;
  midY = height/2;

  theColumn = new ballColumn(100, 100, 100, 36);
}

function draw() {
  background(51);
  for (var i = 0; i < theColumn.balls.length; i++) {
      var color = getBallColor(i + 1, angle);
      theColumn.balls[i].setColor(color);
  }
  theColumn.draw();

  angle += freq;
}

function getBallColor(ballIndex, angle) {
    var curBallOffset = ballIndex * ballOffset;
    var r = colorForAngle(angle * curBallOffset);
    var g = colorForAngle(angle * curBallOffset * colorOffset);
    var b = colorForAngle(angle * curBallOffset * colorOffset * 2);
    return new rgbColor(r, g, b);
}


function colorForAngle(angle) {
  return floor(((255 / 2) * sin(angle)) + (255 / 2));
}

function rgbColor(r,g,b) {
    var self = this;
    self.r = r;
    self.g = g;
    self.b = b;
}

function ball(x, y, _size, _color, label) {
    var self = this;
    self.x = x;
    self.y = y;
    self.size = _size;
    self.label = label;
    self.draw = function() {
        if (self.x > 9000) {
            return;
        }
        fill(self.color.r, self.color.g, self.color.b);
        ellipse(self.x, self.y, self.size, self.size);
        if (self.label) {
            fill(0);
            text(self.label, self.x - 10, self.y - 10);
        }
    };

    self.setColor = function(_color) {
        self.color = _color;
    };

    self.setColor(_color);
}

function ballColumn(x, y, ballSize, numBalls) {
    var self = this;
    self.x = x;
    self.y = y;
    self.balls = [];
    self.numBalls = 0;
    self.numCols = 6;
    self.numRows = 6;

    //there might be a mathy way of doing this but since I'm just simulating
    //a 3D object this seemed expedient. Basically the two balls at the
    //rear are hidden
    self.xposLookup = {
        0: x,
        1: x + (ballSize / 2),
        2: x + ballSize * 1.5,
        3: x + ballSize * 2,
        4: 99999,
        5: 99999
    };

    self.addBall = function() {
        var oldLength = self.balls.length;
        var col = oldLength % self.numCols;
        var row = Math.floor((oldLength) / self.numRows);
        var y = ballSize * row + self.y;
        var x = self.xposLookup[col] + self.x;
        var label = null;
        // label = '' + (oldLength + 1);
        self.balls.push(new ball(x, y, ballSize, new rgbColor(255, 255, 255), label));
        self.numBalls = self.balls.length;
    };

    self.addBalls = function(numBalls) {
          for (var i = 0; i < numBalls; i++) {
              self.addBall();
          }
    };

    self.addBalls(numBalls);

    self.draw = function () {
        var order = [0,3,1,2]; //this is more weird 3D->2D funkiness to get the layers right
        for (var i = 0; i < 6; i ++) {
            for (var j = 0; j < order.length; j++) {
                var index = 6 * i + order[j];
                self.balls[index].draw();
            }
        }
    };
}
