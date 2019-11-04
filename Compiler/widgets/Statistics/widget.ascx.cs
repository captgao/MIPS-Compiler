using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using App_Code.Controls;
using BlogEngine.Core;

namespace Widgets.Statistics
{
   public partial class Widget : WidgetBase
   {
      #region Public Methods

      /// <summary>
      /// This method works as a substitute for Page_Load. You should use this method for
      ///     data binding etc. instead of Page_Load.
      /// </summary>
      public override void LoadWidget()
      {
         var settings = GetSettings();
         lblOnlineUsersCaption.Text = settings["onlineuserstext"] ?? lblOnlineUsersCaption.Text;
         lblGuestUsersCaption.Text = settings["guestuserstext"] ?? lblGuestUsersCaption.Text;
         lblTodayVisitsCaption.Text = settings["todayvisitstext"] ?? lblTodayVisitsCaption.Text;
         lblYesterdayVisitsCaption.Text = settings["yesterdayvisitstext"] ?? lblYesterdayVisitsCaption.Text;
         //lblTodayVisitorsCaption.Text = settings["todayvisitorstext"] ?? lblTodayVisitorsCaption.Text;
         lblTotalVisitsCaption.Text = settings["totalvisitstext"] ?? lblTotalVisitsCaption.Text;
         //lblTotalVisitorsCaption.Text = settings["totalvisitorstext"] ?? lblTotalVisitorsCaption.Text;
         int interval = int.Parse(settings["interval"] ?? "5");

         // first check the data storage in application memory is exist.
         if (Context.Application[StorageKey] != null)
         {
            StResult res;
            if (Cache.Get("StResult") == null || DateTime.Now.Subtract(((StResult)Cache.Get(CacheKey)).LastRefresh).TotalMinutes > interval)
            {
               DataTable table = Context.Application[StorageKey] as DataTable;
               var rawData = table.Rows.OfType<DataRow>().Select(a => new { IP = a["IP"].ToString(), IsAuthenticated = bool.Parse(a["IsAuthenticated"].ToString()), Date = Convert.ToDateTime(a["Date"]) });

               // proccess online users from raw data. all users who requested for last 5 minutes are online.
               var onlineUsers = rawData.Where(a => DateTime.Now.Subtract(a.Date).TotalMinutes <= 5).DistinctBy(a => a.IP);
               var guestUsers = onlineUsers.Where(a => !a.IsAuthenticated);

               // proccess visits from raw data. counts of raw data is equal to visits.
               int visistsCount = rawData.Count();

               // process visitors from raw data. all invidual IPs are equal to visitors.
               //var visitors = rawData.DistinctBy(a => a.IP);

               int dayNo = (int)DateTime.Now.DayOfWeek;
               int yesterdayNo = dayNo - 1 == -1 ? 6 : dayNo - 1;
               string date = DateTime.Now.ToString("yyyy/MM/dd");
               // after results achieved table must be purged and put results into xml storage file.
               StatisticsStorage db = new StatisticsStorage(StFileName);
               StDay day = new StDay()
               {
                  Visits = visistsCount,
                  //Visitors = visitors.Count(),
                  DayNo = dayNo,
                  Date = DateTime.Now
               };
               db.Save(day);

               // after saving analytics data its time to fill results and adds it to cache
               StDay newDay = db.GetDay(dayNo);
               StDay newYesterday = db.GetDay(yesterdayNo);
               res = new StResult()
               {
                  OnlineUsers = onlineUsers.Count() == 0 ? 1 : onlineUsers.Count(), // because the user itself who requested is online. sometimes it returns no users because just one user requested and this code will run first and then HttpModule.
                  GuestUsers = guestUsers.Count(),
                  Visits = newDay.Visits,
                  YesterdayVisits = newYesterday.Visits,
                  //Visitors = newDay.Visitors,
                  TotalVisits = db.Total.TotalVisits,
                  //TotalVisitors = db.Total.TotalVisitors,
                  LastRefresh = DateTime.Now
               };
               Cache.Insert(CacheKey, res, null, DateTime.Now.AddMinutes(interval), System.Web.Caching.Cache.NoSlidingExpiration);

               Context.Application.Lock();
               Context.Application[StorageKey] = null;
               Context.Application.UnLock();
            }
            else
            {
               res = ((StResult)Cache.Get(CacheKey));
            }

            lblOnlineUsers.Text = res.OnlineUsers.ToString();
            lblGuestUsers.Text = res.GuestUsers.ToString();
            lblTodayVisits.Text = res.Visits.ToString();
            lblYesterdayVisits.Text = res.YesterdayVisits.ToString();
            //lblTodayVisitors.Text = res.Visitors.ToString();
            lblTotalVisits.Text = res.TotalVisits.ToString();
            //lblTotalVisitors.Text = res.TotalVisitors.ToString();
         }
      }

      #endregion

      #region Constants and Fields
      public readonly string StorageKey = "Shakib";
      public readonly string CacheKey = "StResult";
      #endregion

      #region Properties

      /// <summary>
      ///     Gets a value indicating whether IsEditable.
      /// </summary>
      public override bool IsEditable
      {
         get
         {
            return true;
         }
      }

      /// <summary>
      ///     Gets Name.
      /// </summary>
      public override string Name
      {
         get
         {
            return "Statistics";
         }
      }

      public string StFileName
      {
         get
         {
            string p = Path.Combine(Blog.CurrentInstance.StorageLocation, "Statistics.xml");
            return System.Web.Hosting.HostingEnvironment.MapPath(p);
         }
      }

      #endregion
   }

   public class StatisticsStorage
   {
      private XDocument Doc;
      private readonly string FileName;
      public StatisticsStorage(string xmlFileName)
      {
         FileName = xmlFileName;
         string defaultFile = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>\r\n<Statistics>\r\n  <Weekly>\r\n    <Day DayNo=\"0\" Date=\"2013/01/01\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"1\" Date=\"2013/01/02\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"2\" Date=\"2013/01/03\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"3\" Date=\"2013/01/04\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"4\" Date=\"2013/01/05\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"5\" Date=\"2013/01/06\" Visits=\"0\" Visitors=\"0\" />\r\n    <Day DayNo=\"6\" Date=\"2013/01/07\" Visits=\"0\" Visitors=\"0\" />\r\n  </Weekly>\r\n  <Total TotalVisits=\"0\" TotalVisitors=\"0\" />\r\n  <Settings>\r\n    <LastUpdate DateTime=\"2013/01/01 20:10:30\" />\r\n  </Settings>\r\n</Statistics>";
         if (!File.Exists(FileName))
         {
            File.WriteAllText(FileName, defaultFile, Encoding.UTF8);
         }
         Doc = XDocument.Load(FileName);
      }

      public StDay GetDay(int dayNo)
      {
         XElement weekly = Doc.Descendants("Weekly").First();
         return weekly.Elements().Where(a => a.Attribute("DayNo").Value.Equals(dayNo.ToString())).Select(a => new StDay()
         {
            DayNo = int.Parse(a.Attribute("DayNo").Value),
            Date = DateTime.Parse(a.Attribute("Date").Value),
            Visits = int.Parse(a.Attribute("Visits").Value),
            //Visitors = int.Parse(a.Attribute("Visitors").Value)
         }).First();
      }

      public StTotal Total
      {
         get
         {
            XElement total = Doc.Descendants("Total").First();
            return new StTotal()
            {
               TotalVisits = int.Parse(total.Attribute("TotalVisits").Value),
               //TotalVisitors = int.Parse(total.Attribute("TotalVisitors").Value)
            };
         }
         set
         {
            XElement total = Doc.Descendants("Total").First();
            total.Attribute("TotalVisits").SetValue(Total.TotalVisits + value.TotalVisits);
            //total.Attribute("TotalVisitors").SetValue(Total.TotalVisitors + value.TotalVisitors);
         }
      }

      public DateTime LastUpdate
      {
         get
         {
            return DateTime.Parse(Doc.Descendants("Settings").First().Element("LastUpdate").Attribute("DateTime").Value);
         }
         set
         {
            Doc.Descendants("Settings").First().Element("LastUpdate").Attribute("DateTime").SetValue(value.ToString("yyyy/MM/dd HH:mm:ss"));
         }
      }

      public void Save(StDay day)
      {
         XElement weekly = Doc.Descendants("Weekly").First();
         var xDay = weekly.Elements().Where(a => a.Attribute("DayNo").Value.Equals(day.DayNo.ToString())).First();
         StDay resDay = GetDay(day.DayNo);

         int dateRes = day.Date.Date.CompareTo(resDay.Date.Date);
         if (dateRes == 0)
         {
            xDay.Attribute("DayNo").SetValue(day.DayNo);
            xDay.Attribute("Date").SetValue(day.Date.ToString("yyyy/MM/dd"));
            xDay.Attribute("Visits").SetValue(resDay.Visits + day.Visits);
            //xDay.Attribute("Visitors").SetValue(resDay.Visitors + day.Visitors);
         }
         else
         {
            xDay.Attribute("DayNo").SetValue(day.DayNo);
            xDay.Attribute("Date").SetValue(day.Date.ToString("yyyy/MM/dd"));
            xDay.Attribute("Visits").SetValue(day.Visits);
            //xDay.Attribute("Visitors").SetValue(day.Visitors);
         }

         Total = new StTotal() { TotalVisits = day.Visits };//, TotalVisitors = day.Visitors };
         LastUpdate = DateTime.Now;

         Doc.Save(FileName);
      }
   }

   public class StDay
   {
      public int DayNo { get; set; }
      public DateTime Date { get; set; }
      public int Visits { get; set; }
      //public int Visitors { get; set; }
   }

   public class StTotal
   {
      public int TotalVisits { get; set; }
      //public int TotalVisitors { get; set; }
   }

   class StResult
   {
      public DateTime LastRefresh { get; set; }
      public int OnlineUsers { get; set; }
      public int GuestUsers { get; set; }
      public int Visits { get; set; }
      public int YesterdayVisits { get; set; }
      //public int Visitors { get; set; }
      public int TotalVisits { get; set; }
      //public int TotalVisitors { get; set; }
   }

   public static class TempMethods
   {
      public static IEnumerable<TSource> DistinctBy<TSource, TKey>(this IEnumerable<TSource> source, Func<TSource, TKey> keySelector)
      {
         HashSet<TKey> seenKeys = new HashSet<TKey>();
         foreach (TSource element in source)
         {
            if (seenKeys.Add(keySelector(element)))
            {
               yield return element;
            }
         }
      }
   }
}