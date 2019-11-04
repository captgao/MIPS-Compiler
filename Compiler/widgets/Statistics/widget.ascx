<%@ Control Language="C#" AutoEventWireup="true" CodeFile="widget.ascx.cs" Inherits="Widgets.Statistics.Widget" %>

<div id="statisticsView">
    <asp:Label ID="lblOnlineUsersCaption" Text="Online Users: " runat="server" />
    <asp:Label ID="lblOnlineUsers" Text="0" runat="server" />
    <br />                       
    <asp:Label ID="lblGuestUsersCaption" Text="Guest Users: " runat="server" />
    <asp:Label ID="lblGuestUsers" Text="0" runat="server" />
    <br />   
    <asp:Label ID="lblTodayVisitsCaption" Text="Today Visits: " runat="server" />
    <asp:Label ID="lblTodayVisits" Text="0" runat="server" />
    <br />   
    <asp:Label ID="lblYesterdayVisitsCaption" Text="Yesterday Visits: " runat="server" />
    <asp:Label ID="lblYesterdayVisits" Text="0" runat="server" />
<%--    <br />                       
    <asp:Label ID="lblTodayVisitorsCaption" Text="Today Visitors: " runat="server" />
    <asp:Label ID="lblTodayVisitors" Text="0" runat="server" />--%>
    <br />                       
    <asp:Label ID="lblTotalVisitsCaption" Text="Total Visits: " runat="server" />
    <asp:Label ID="lblTotalVisits" Text="0" runat="server" />
<%--    <br />                       
    <asp:Label ID="lblTotalVisitorsCaption" Text="Total Visitors: " runat="server" />
    <asp:Label ID="lblTotalVisitors" Text="0" runat="server" />--%>
    <br />
</div>