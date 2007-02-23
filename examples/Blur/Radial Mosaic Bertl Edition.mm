# by Herbert Poetzl
filter rmbe (image in, float pixel_area: 10-5000, float aspect_ratio: 0.1-10)
    A=pixel_area;
    asp=aspect_ratio;
    B=(A*asp)^0.5;
    pi2=pi/2;
    twopi=pi*2;
    n=floor(r/B)+0.5;
    alpha=twopi/floor(twopi/(A/(pi2*(n-0.5)*B*B)));
    in(ra:[(n*A/(alpha*pi2))^0.5,a-a%alpha])
end