require( "iuplua" )
require( "iupluacontrols" )
require( "iuplua_pplot"  )

function createPlot(title, xlable, ylabel)
	local plot = iup.pplot{TITLE = title,
                    MARGINBOTTOM="100",
                    MARGINLEFT="100",
                    AXS_XLABEL= xlable,
                    AXS_YLABEL= ylabel,
					AXS_YAUTOMIN = "YES",
					AXS_XAUTOMIN = "YES",
					AXS_YAUTOMAX = "YES",
					AXS_XAUTOMAX = "YES",
					LEGENDSHOW = "YES",
                    ["USE_GDI+"] = "YES",}

	function startAdd()
		iup.PPlotBegin(plot, 0)
	end

	function addData(x,y)
		--should check x y type here, must be numbers
		iup.PPlotAdd(plot,x,y)
	end

	function finishAdd(legend)
		iup.PPlotEnd(plot)
		plot.DS_MODE = "MARKLINE"
		plot.DS_LEGEND = legend
		plot.REDRAW = "YES"
	end

	function getPlot()
		return plot
	end
end

plots = {}

function show(wndTitle)
	vbox = iup.vbox{}
	for k,v in pairs(plots) do
		iup.Append(vbox, v)
	end
	dlg = iup.dialog{vbox, title=wndTitle, size="THIRDxHALF"}
	dlg:show()
	iup.MainLoop()
end

function finishPlot()
	table.insert(plots,getPlot())
end

--main chunk
--
createPlot("Plot1", "x", "y")
	startAdd()
		addData(0,0) addData(1,2); addData(16,4) addData(26,34)
	finishAdd("p1")

	startAdd()
		addData(0,0) addData(2,3); addData(12,12)
	finishAdd("p2")
finishPlot()

--
createPlot("Plot2", "x", "y")
	startAdd()
		addData(0,0) addData(1,2); addData(13,21)
	finishAdd("t1")

	startAdd()
		addData(0,0) addData(2,3); addData(12,12)
	finishAdd("t2")
finishPlot()

--
createPlot("Plot3", "x", "y")
	startAdd()
		addData(0,0) addData(12,2); addData(17,21)
	finishAdd("c1")

	startAdd()
		addData(0,0) addData(23,3); addData(25,12)
	finishAdd("c2")
finishPlot()

show("Simple Profiler")

