document.addEventListener("DOMContentLoaded", function() {
    setTimeout(function() {
        var frame = document.getElementById("frame");
        var image = document.getElementById("image");
        var rotate = document.getElementById("rotate");
        var rotation = 0;

        // set frame size
        frame.style.height = Math.max(image.clientHeight, image.naturalHeight, image.clientWidth, image.naturalWidth) + 'px';

        // rotate on button click
        rotate.addEventListener("click", function() {
            rotation = (rotation + 90) % 360;
            image.style.transform = 'rotate(' + rotation + 'deg)';
        });
    }, 1000);
});