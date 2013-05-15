require( "iuplua" )
require( "iupluacontrols" )
require( "iuplua_pplot"  )

function createPlot()
	local plot = iup.pplot{TITLE = "Steps Comparison",
                    MARGINBOTTOM="55",
                    MARGINLEFT="75",
                    AXS_XLABEL="Items in the array",
                    AXS_YLABEL="Operation Cost(steps)",
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
		return plot
	end

end

plots = {}

function show(p)
	vbox = iup.vbox{}
	for k,v in pairs(p) do
		iup.Append(vbox, v)
	end
	dlg = iup.dialog{vbox, title="A Simple Profiler [By Coder_In_Zen_Mode]",size="THIRDxHALF"}
	dlg:show()
	iup.MainLoop()
end

--main chunk
createPlot()
startAdd()
addData(0,0) addData(1,2); addData(16,4) addData(26,34)
finishAdd("p1")

startAdd()
addData(0,0) addData(2,3); addData(12,12)
table.insert(plots, finishAdd("p2"))

createPlot()
startAdd()
addData(0,0) addData(1,2); addData(13,21)
finishAdd("t1")

startAdd()
addData(0,0) addData(2,3); addData(12,12)
table.insert(plots, finishAdd("t2"))

show(plots)

