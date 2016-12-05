
var angle = 0.0;
var freq = 0.01;
var colorOffset = 1.5;
var ballOffset = 1.5;
var midX = 0;
var midY = 0;
var numBalls = 36;
var theColumn = null;



setup = function() {
  createCanvas(2000,2000);
  noStroke();
  midX = width/2;
  midY = height/2;

  theColumn = new ballColumn(0, 0, 100, 36);
};

draw = function() {
  background(51);
  theColumn.draw();
  // drawBall(angle, 1, 100);
  // drawBall(angle, 2, 100);
  // drawBall(angle, 3, 100);
  //
  //
  // angle += freq;
};


function colorForAngle(angle) {
  return floor(((255 / 2) * sin(angle)) + (255 / 2));
}

function drawBall(angle, ballIndex, ballSize) {
  var curBallOffset = ballIndex * ballOffset;
  var r = colorForAngle(angle * curBallOffset);
  var g = colorForAngle(angle * curBallOffset * colorOffset);
  var b = colorForAngle(angle * curBallOffset * colorOffset * 2);
  var color = new rgbColor(r, g, b);
  var the_ball = new ball(20 + ballIndex * ballSize, midY, ballSize, color);
  the_ball.draw();
  // fill(r,g,b);
  // ellipse(20 + ballIndex * ballSize, midY, ballSize, ballSize);
}

function rgbColor(r,g,b) {
    var self = this;
    self.r = r;
    self.g = g;
    self.b = b;
}

function ball(x, y, _size, _color) {
    var self = this;
    self.x = x;
    self.y = y;
    self.size = _size;
    self.draw = function() {
        if (self.x > 9000) {
            return;
        }
        fill(self.color.r, self.color.g, self.color.b);
        ellipse(self.x, self.y, self.size, self.size);
        // console.log('color:');
        // console.log(self.color);
        // console.log('pos:' + self.x + ',' + self.y);
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


    self.xposLookup = {
        0: 99999,
        1: x,
        2: x + (ballSize / 2),
        3: x + ballSize,
        4: x + ballSize * 2,
        5: 99999
    };

    self.addBall = function() {
        var mod = self.balls.length === 0 ? 0 : (self.balls.length + 1) % 6;
        var row = mod + 1;
        var col = mod;
        var y = ballSize * row + self.y;
        var x = self.xposLookup[col] + self.x;
        self.balls.push(new ball(x, y, ballSize, new rgbColor(255, 255, 255)));
        self.numBalls = self.balls.length;
    };

    self.addBalls = function(numBalls) {
          for (i = 0; i < numBalls; i++) {
              self.addBall();
          }
    };

    self.addBalls(numBalls);

    self.draw = function () {
        for (i = 0; i < self.balls.length; i++) {
            self.balls[i].draw();
        }
    };
}
