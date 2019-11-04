using System;
using App_Code.Controls;

namespace Widgets.Statistics
{
   public partial class Edit : WidgetEditBase
   {
      #region Public Methods

      /// <summary>
      /// Saves this the basic widget settings such as the Title.
      /// </summary>
      public override void Save()
      {
         var settings = GetSettings();
         settings["onlineuserstext"] = txtOnlineUsers.Text;
         settings["guestuserstext"] = txtGuestUsers.Text;
         settings["todayvisitstext"] = txtTodayVisits.Text;
         settings["yesterdayvisitstext"] = txtYesterdayVisits.Text;
         //settings["todayvisitorstext"] = txtTodayVisitors.Text;
         settings["totalvisitstext"] = txtTotalVisits.Text;
         //settings["totalvisitorstext"] = txtTotalVisitors.Text;
         settings["interval"] = txtInterval.Text;
         SaveSettings(settings);
      }

      protected override void OnInit(EventArgs e)
      {
         var settings = GetSettings();
         txtOnlineUsers.Text = settings["onlineuserstext"] ?? txtOnlineUsers.Text;
         txtGuestUsers.Text = settings["guestuserstext"] ?? txtGuestUsers.Text;
         txtTodayVisits.Text = settings["todayvisitstext"] ?? txtTodayVisits.Text;
         txtYesterdayVisits.Text = settings["yesterdayvisitstext"] ?? txtYesterdayVisits.Text;
         //txtTodayVisitors.Text = settings["todayvisitorstext"] ?? txtTodayVisitors.Text;
         txtTotalVisits.Text = settings["totalvisitstext"] ?? txtTotalVisits.Text;
         //txtTotalVisitors.Text = settings["totalvisitorstext"] ?? txtTotalVisitors.Text;
         txtInterval.Text = settings["interval"] ?? txtInterval.Text;

         base.OnInit(e);
      }
      #endregion

   }
}