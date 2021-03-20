%for i=180:255

black = zeros(2,2);
white = ones(2,2);
white(1:2, 1:2) = 255;

seg = zeros(4,4);

seg(1:2,1:2) = white;
seg(3:4,1:2) = black;
seg(1:2,3:4) = black;
seg(3:4,3:4) = white;

line=zeros(16,64);
line = [seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg;seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg;seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg;seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg,seg];
line2 = zeros(16,256);
line2 = [line,line,line,line];

img= zeros(256,256);

gray_img = zeros(16,256);
gray_img(1:16,1:256) = 210;

img=[line2;gray_img;line2;gray_img;line2;gray_img;line2;gray_img;line2;gray_img;line2;gray_img;line2;gray_img;line2;gray_img];
imshow(img,[0,256]),title('Value: 210'),colormap('gray');

%pause(0.5);
%end