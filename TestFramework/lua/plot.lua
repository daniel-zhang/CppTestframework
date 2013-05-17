require( "iuplua" )
require( "iupluacontrols" )
require( "iuplua_pplot"  )

function createPlot(title, xlable, ylabel)
	local plot = iup.pplot{TITLE = title,
                    MARGINBOTTOM="50",
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
		plot.DS_MARKSTYLE="CIRCLE"
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
	dlg = iup.dialog{vbox, title=wndTitle,size="FULLxFULL"}
	dlg:show()
	iup.MainLoop()
end

function finishPlot()
	table.insert(plots,getPlot())
end
