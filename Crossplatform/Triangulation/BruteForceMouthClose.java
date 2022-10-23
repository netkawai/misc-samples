
/*-------------------------------------------------------------------*/
/*  BruteForceMouthClose.java                                        */
/*      This file contains the class needed for the implementation   */
/*      of mouth closing for a simple polygon.                       */
/*                                                                   */
/*- Modification History---------------------------------------------*/
/*  When:       Who:          Comments:                              */
/*                                                                   */
/*  97.12.09    Ian Garton    Final Implementation                   */
/*-------------------------------------------------------------------*/


import java.awt.*;
import java.applet.Applet;


public class BruteForceMouthClose extends Applet
implements CanvasController
{
    private PolygonCanvas canvas;

    private static boolean runsInBrowser = true;
    private Button quitButton, clearButton, closeMouthButton;

    private int npoints;
    private int mouthCounter;
    private int concaveCount;
    private double xpoints[], ypoints[];
    private double xpointsTemp[], ypointsTemp[];
    private int ptType[];
    private Polygon transformedPoly;
    private Polygon mouthPoly[];

    
    public void init()
    {
	setLayout(new BorderLayout());

	canvas = new PolygonCanvas();
	canvas.setFinishNotify(this);
	add("Center", canvas);	

	Panel p = new Panel();

	if (!runsInBrowser)
	    p.add(quitButton = new Button("Quit"));

	p.add(clearButton = new Button("Clear"));
	p.add(closeMouthButton = new Button("Close a Mouth!"));
	closeMouthButton.disable();
	
	add("South", p);

	resize (400,400);
        validate();
    }


    /*
     * main:  This is to run outside of Netscape, using AppletFrame.
     */
    public static void main (String args[])
    {
	runsInBrowser = false;
	AppletFrame.startApplet("BruteForceMouthClose",
				"Brute Force Mouth Closing",
				args);
    }


    /* finishNotify:  When user enters final point, variables are
     *                initialized and the Close Mouth button is made
     *                active.
     */
    public void finishNotify(Canvas c)
    {
	PolygonCanvas pc = (PolygonCanvas)c;
	npoints = pc.inputPolygon.npoints;
	mouthCounter = 0;
	xpoints = new double[npoints];
	ypoints = new double[npoints];
	mouthPoly = new Polygon[npoints-2];
	for (int i = 0; i < npoints - 2; i++)
	    mouthPoly[i] = new Polygon();
	
	for (int i = 0; i < npoints; i++)
	{
	    xpoints[i] = pc.inputPolygon.xpoints[i];
	    ypoints[i] = pc.inputPolygon.ypoints[i];
	}
	for (int i = 0; i < npoints-2; i++)
	    mouthPoly[i] = null;
	transformedPoly = null;
	if (npoints > 4) 
	    closeMouthButton.enable();
    }


    /* action:  Performs actions according to button user activates.
     */	
    public boolean action(Event e, Object arg)
    {
	int old_npoints;

	if (e.target == quitButton) {
	    System.exit (0);
	} else if (e.target == clearButton) {
	    closeMouthButton.disable();
	    for (int i = 0; i < npoints - 2; i++)
	        mouthPoly[i] = null;
	    transformedPoly = null;
	    mouthCounter = 0;
	    canvas.reset();
	} else if (e.target == closeMouthButton) {
	    old_npoints = npoints;
	    if (npoints > 4) {
	        classifyPoints();
	        doCloseMouth();
	        canvas.repaint();
	    }
	    if (concaveCount == 0) { 
		closeMouthButton.disable();
	    }
	} else
	    return false;
	return true;
    }


    /* paint:  Displays modified polygon
     */
    public void paint(Canvas caller, Graphics g)
    {
	if (transformedPoly == null) return;

	for (int i = 0; i < mouthCounter; i++) {
	    g.setColor(Color.yellow);
	    g.fillPolygon(mouthPoly[i]);
	    g.setColor(Color.pink);
	    g.drawPolygon(mouthPoly[i]);
	}

	g.setColor(Color.black);
	g.drawPolygon(transformedPoly);
    }


    /* polygonCounterClockwise:  Returns false if user inputted polygon in 
     *                           clockwise order, true if counterclockwise.
     *                           The Law of Cosines is used to determine the
     *                           angle.
     */
    boolean polygonCounterClockwise()
    {
	double aa, bb, cc, b, c, theta;
	double concave_turn;
	double concave_sum = 0;

	for (int i = 0; i < npoints - 2; i++) {
	    aa = ((xpoints[i+2] - xpoints[i]) * (xpoints[i+2] - xpoints[i])) +
	         ((-ypoints[i+2] + ypoints[i]) * (-ypoints[i+2] + ypoints[i]));
	    
	    bb = ((xpoints[i+1] - xpoints[i]) * (xpoints[i+1] - xpoints[i])) +
	         ((-ypoints[i+1] + ypoints[i]) * (-ypoints[i+1] + ypoints[i]));
	    
	    cc = ((xpoints[i+2] - xpoints[i+1]) * 
		  (xpoints[i+2] - xpoints[i+1])) +
		 ((-ypoints[i+2] + ypoints[i+1]) *
		  (-ypoints[i+2] + ypoints[i+1]));
	
	    b = Math.sqrt(bb);
	    c = Math.sqrt(cc);
	    theta = Math.acos((bb + cc - aa) / (2 * b * c));

	    if (concave(xpoints[i], ypoints[i],
                       xpoints[i+1], ypoints[i+1],
                       xpoints[i+2], ypoints[i+2])) {
		concave_turn = Math.PI - theta;
		concave_sum += concave_turn;
	    }
	    else {
		concave_sum -= Math.PI - theta;
	    }
	}
	aa = ((xpoints[1] - xpoints[npoints-2]) * 
	      (xpoints[1] - xpoints[npoints-2])) +
	     ((-ypoints[1] + ypoints[npoints-2]) * 
	      (-ypoints[1] + ypoints[npoints-2]));
	    
	bb = ((xpoints[0] - xpoints[npoints-2]) * 
	      (xpoints[0] - xpoints[npoints-2])) +
	     ((-ypoints[0] + ypoints[npoints-2]) * 
	      (-ypoints[0] + ypoints[npoints-2]));
	    
	cc = ((xpoints[1] - xpoints[0]) * (xpoints[1] - xpoints[0])) +
	     ((-ypoints[1] + ypoints[0]) * (-ypoints[1] + ypoints[0]));
	
	b = Math.sqrt(bb);
	c = Math.sqrt(cc);
	theta = Math.acos((bb + cc - aa) / (2 * b * c));
	
	if (concave(xpoints[npoints-2], ypoints[npoints-2],
                   xpoints[0], ypoints[0],
                   xpoints[1], ypoints[1])) {
	    concave_turn = Math.PI - theta;
	    concave_sum += concave_turn;
	}
	else { 
	    concave_sum -= Math.PI - theta;
	}

	if (concave_sum >= (2 * 3.14159)) 
	    return false;
	else 
	    return true;
    }


    /* classifyPoints:  Classifies points as "convex" or "concave".  
     *                  Concave points are represented as a "1" in the
     *                  ptType array; convex points are represented as a
     *                  "-1" in the array.
     */
    void classifyPoints()
    {
	ptType = new int[npoints];
	xpointsTemp = new double[npoints];
	ypointsTemp = new double[npoints];
	concaveCount = 0;

	/* Before cutting any ears, we must determine if the polygon was
         * inputted in clockwise order or not, since the algorithm for
         * closing mouths assumes that the polygon's points are not in 
         * clockwise order.  If the points are in clockwise order, they
         * are simply reversed in the array.
         */
	if (mouthCounter == 0) {
	    if (!polygonCounterClockwise()) {
	        for (int i = 0; i < npoints; i++) {
		    xpointsTemp[i] = xpoints[npoints-1 - i];
		    ypointsTemp[i] = ypoints[npoints-1 - i];
	        }
	        for (int i = 0; i < npoints - 1; i++) {
		    xpoints[i] = xpointsTemp[i];
		    ypoints[i] = ypointsTemp[i];
		}
	    }
	}

        for (int i = 0; i < npoints - 1; i++) {
	    if (i == 0) {
		if (concave(xpoints[npoints-2], ypoints[npoints-2],
                            xpoints[i], ypoints[i],
                            xpoints[i+1], ypoints[i+1])) {
		    ptType[i] = 1;	/* point is concave */
		    concaveCount++;
		}
		else {
		    ptType[i] = -1;	/* point is convex */
	        }
	    }
	    else {    /* i > 0 */
		if (concave(xpoints[i-1], ypoints[i-1],
                            xpoints[i], ypoints[i],
                            xpoints[i+1], ypoints[i+1])) {
		    ptType[i] = 1;	/* point is concave */
 		    concaveCount++;
                }
		else {
		    ptType[i] = -1;	/* point is convex */		    
		}
	    }
	}
    }


    /* concave:  returns true if point (x2, y2) is concave
     */
    boolean concave(double x1, double y1, double x2, double y2, 
		    double x3, double y3)
    {
	if (area(x1, y1, x2, y2, x3, y3) < 0)
	    return true;
	else
	    return false;
    }


    /* area:  determines area of triangle formed by three points
     */
    double area(double x1, double y1, double x2, double y2,
		double x3, double y3)
    {
	double areaSum = 0;

	areaSum += x1 * (y3 - y2);
	areaSum += x2 * (y1 - y3);
	areaSum += x3 * (y2 - y1);

	/* for actual area, we need to multiple areaSum * 0.5, but we are
         * only interested in the sign of the area (+/-)
         */

	return areaSum;
    }


    /* triangleContainsPoints:  returns true if the triangle formed by
     *                          three points contains another point
     */
    boolean triangleContainsPoint(double x1, double y1, double x2,
				  double y2, double x3, double y3)
    {
	int i = 0;
	double area1, area2, area3;
	boolean noPointInTriangle = true;

	while ((i < npoints - 1) && (noPointInTriangle)) {
	    if ((ptType[i] == -1)   /* point is convex */  &&
                (((xpoints[i] != x1) && (ypoints[i] != y1)) ||
		 ((xpoints[i] != x2) && (ypoints[i] != y2)) ||
		 ((xpoints[i] != x3) && (ypoints[i] != y3)))) {

		area1 = area(x1, y1, x2, y2, xpoints[i], ypoints[i]);
		area2 = area(x2, y2, x3, y3, xpoints[i], ypoints[i]);
		area3 = area(x3, y3, x1, y1, xpoints[i], ypoints[i]);

		if (area1 > 0)
		    if ((area2 > 0) && (area3 > 0))
			noPointInTriangle = false;
		if (area1 < 0)
		    if ((area2 < 0) && (area3 < 0))
			noPointInTriangle = false;
	    }
	    i++;
	}
	return !noPointInTriangle;
    }


    /* mouth:  returns true if the point (x2, y2) is a mouth, 
     *         false otherwise
     */
    boolean mouth(double x1, double y1, double x2, double y2, 
                double x3, double y3)
    {
	if (concaveCount != 0)
	    if (triangleContainsPoint(x1, y1, x2, y2, x3, y3))
		return false;
	    else 
		return true;
	else
	    return true;
    }


    /* closeMouth:  creates triangle that represents mouth for graphics 
     *              purposes
     */
    void closeMouth(int index)
    {
	Polygon p = new Polygon();
	int new_i = 0;
	int i = 0;

	double mx[], my[];
	mx = new double[4];
	my = new double[4];

	if (index == 0) {
	    mx[0] = xpoints[npoints-2];
	    my[0] = ypoints[npoints-2];
	    mx[1] = xpoints[index];
	    my[1] = ypoints[index];
	    mx[2] = xpoints[index+1];
	    my[2] = ypoints[index+1];
	}
	else if ((index > 0) && (index < npoints-2)) {
	    mx[0] = xpoints[index-1];
	    my[0] = ypoints[index-1];
	    mx[1] = xpoints[index];
	    my[1] = ypoints[index];
	    mx[2] = xpoints[index+1];
	    my[2] = ypoints[index+1];
	}
	else if (index == npoints-2) {
	    mx[0] = xpoints[index-1];
	    my[0] = ypoints[index-1];
	    mx[1] = xpoints[index];
	    my[1] = ypoints[index];
	    mx[2] = xpoints[0];
	    my[2] = ypoints[0];
	}
	for (i = 0; i < 3; ++i)
	    p.addPoint((int) mx[i], (int) my[i]);
	mx[3] = mx[0];
	my[3] = my[0];
	p.addPoint((int) mx[0], (int) my[0]);
	mouthPoly[mouthCounter] = p;
	mouthCounter++;
    }


    /* updatePolygon:  creates new polygon without the mouth that was
     *                 closed
     */
    void updatePolygon(int index)
    {
	Polygon p = new Polygon();
	int new_i = 0;
	int i = 0;

	if (index == 0)
	    i++;
	while (i < npoints - 1) {
	    if (i == index)
		i++;
	    if (i < npoints - 1) {
		xpoints[new_i] = xpoints[i];
		ypoints[new_i] = ypoints[i];
		p.addPoint((int) xpoints[new_i], (int) ypoints[new_i]);
		new_i++;
		i++;
	    }
	}
	xpoints[npoints-2] = xpoints[0];
	ypoints[npoints-2] = ypoints[0];
	p.addPoint((int) xpoints[0], (int) ypoints[0]);
	npoints--;
	transformedPoly = p;
    }


    /* doCloseMouth:  Performs all the functions needed to find and close
     *                a mouth.
     */
    void doCloseMouth()
    {
	boolean mouthHasBeenClosed = false;
	int i = 0;

	while ((i < npoints - 1) && (!mouthHasBeenClosed)) {
	    if (ptType[i] == 1) {  /* point is concave */
		if (i == 0) {
		    if (mouth(xpoints[npoints-2], ypoints[npoints-2],
			      xpoints[i], ypoints[i],
                              xpoints[i+1], ypoints[i+1])) {
			closeMouth(i);
			updatePolygon(i);
			mouthHasBeenClosed = true;
		    }
		}
		else {    /* i > 0 */
		    if (mouth(xpoints[i-1], ypoints[i-1],
			      xpoints[i], ypoints[i],
                              xpoints[i+1], ypoints[i+1])) {
			closeMouth(i);
			updatePolygon(i);
			mouthHasBeenClosed = true;
		    }
		}
	    }
	    i++;
	}
    }
}


