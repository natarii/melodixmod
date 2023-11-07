$fn = 16;
difference() {
    union() {
        cube([6,4,10]);
        translate([0,0,10]) cube([6,4,4]);
    }
    translate([0,0,10]) cube([4,4,1.6]);
    translate([4,0,10.75]) rotate([-90,0,0]) cylinder(r=1, h=99, center=true);
}