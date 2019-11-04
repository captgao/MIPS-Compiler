<%@ Control Language="C#" AutoEventWireup="true" CodeFile="edit.ascx.cs" Inherits="Widgets.Statistics.Edit" %>

<style type="text/css">
    #statisticsEdit{
        width:100%;
    }
    #statisticsEdit div{
        padding:10px;
    }
    #statisticsEdit #captions span {
        display: inline-block;
        width: 160px;
        border-bottom:1px dashed silver;
    }
    #statisticsEdit #captions input{
        width:200px;
    }
</style>

<div id="statisticsEdit">
    <div id="captions">
        <span>Online Users Caption: </span>
        <asp:TextBox ID="txtOnlineUsers" Text="Online Users: " runat="server" />
        <br />
        <span>Guest Users Caption: </span>
        <asp:TextBox ID="txtGuestUsers" Text="Guest Users: " runat="server" />
        <br />
        <span>Today Visits Caption: </span>
        <asp:TextBox ID="txtTodayVisits" Text="Today Visits: " runat="server" />
        <br />
        <span>Yesterday Visits Caption: </span>
        <asp:TextBox ID="txtYesterdayVisits" Text="Yesterday Visits: " runat="server" />
<%--        <br />
        <span>Today Visitors Caption: </span>
        <asp:TextBox ID="txtTodayVisitors" Text="Today Visitors: " runat="server" />--%>
        <br />
        <span>Total Visits Caption: </span>
        <asp:TextBox ID="txtTotalVisits" Text="Total Visits: " runat="server" />
<%--        <br />
        <span>Total Visitors Caption: </span>
        <asp:TextBox ID="txtTotalVisitors" Text="Total Visitors: " runat="server" />--%>
    </div>
    <div id="settings">
        <span>Refresh & Dump Analytics data, Interval: </span>
        <asp:TextBox ID="txtInterval" Text="5" runat="server" Width="40px" />
        <span>&nbsp;Minutes</span>
    </div>
</div>
