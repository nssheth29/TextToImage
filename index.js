var path = require('path'),
tti = require('./build/Release/texttoimage');

var TextToImage = function(text) {
  if (!(this instanceof TextToImage)) {
    return new TextToImage(text);
  }

  this.options = {
    text: text,
    font: 'Arial',
    fontsize: 60,
    bold: false,
    italic: false,

    r: 0,
    g: 0,
    b: 0,
    a: 0,

    shadow: false,
    shadowR: 0,
    shadowG: 0,
    shadowB: 0,
    shadowA: 0,
    shadowOffsetX: 2,
    shadowOffsetY: 2
  };
}

module.exports = TextToImage;

TextToImage.prototype.font = function (family, size, bold, italic) {
  this.options.font = family;
  this.options.fontsize = size;
  this.options.bold = bold;
  this.options.italic = italic;
  return this;
}

TextToImage.prototype.color = function (r, g, b, a) {
  this.options.r = r;
  this.options.g = g;
  this.options.b = b;
  this.options.a = a;
  return this;
}

TextToImage.prototype.applyShadow = function (offsetX, offsetY, r, g, b, a) {
  this.options.shadow = true;
  this.options.shadowOffsetX = offsetX;
  this.options.shadowOffsetY = offsetY;
  this.options.shadowR = r;
  this.options.shadowG = g;
  this.options.shadowB = b;
  this.options.shadowA = a;
  return this;
}

TextToImage.prototype.toImage = function (callback) {
  tti.apply(this.options, callback);
}
