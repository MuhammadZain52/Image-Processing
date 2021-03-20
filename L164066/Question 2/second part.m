img = imread ('C:\Users\Saddam\Desktop\Digital Image processing\Project 2\Data\linear\linear.tif');
imshow(img);

gamma = 3.57;
corLinearImg = 255*(double(img)/255).^(double(1/3.57));
figure(2);
image(uint8(corLinearImg)+1);
set(gca, 'Box', 'off');
axis('image');

graymap = [0:255;0:255;0:255]'/255;
colormap(graymap);