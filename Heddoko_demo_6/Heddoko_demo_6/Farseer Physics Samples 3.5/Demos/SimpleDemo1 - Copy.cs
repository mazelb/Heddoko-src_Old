using System.Text;
using System.IO.Ports;
using FarseerPhysics.Dynamics;
using FarseerPhysics.Factories;
using FarseerPhysics.Samples.Demos.Prefabs;
using FarseerPhysics.Samples.DrawingSystem;
using FarseerPhysics.Samples.ScreenSystem;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Timers;

namespace FarseerPhysics.Samples.Demos
{
    internal class SimpleDemo1 : PhysicsGameScreen, IDemoScreen
    {
        private Border _border;
        private Body _rectangle;
        private Sprite _rectangleSprite;
        private float _rotationAngle;

        #region Arduino communications 
        
        SerialPort _port;
        private string _lastData = "";

        /// <summary>
        /// StartConnectionToArduino.
        /// </summary>
        private void StartConnectionToArduino()
        {
            //  Signal the Arduino Board to start sending data
            // Set up the serial port communication with the Arduino on COM4 at 9600 baud
            _port = new SerialPort("COM3") { BaudRate = 9600 };

            //  hook up the event for receiving the data
            _port.DataReceived += _port_DataReceived;

            //open the port communication
            _port.Open();

            //Fake data
            //System.Timers.Timer aTimer = new System.Timers.Timer();
            //aTimer.Elapsed += new ElapsedEventHandler(_port_DataReceived_timer);
            //aTimer.Interval = 500;
            //aTimer.Enabled = true;

        }

        /// <summary>
        /// EndConnectionToArduino.
        /// </summary>
        private void EndConnectionToArduino()
        {
            //  Signal the Arduino Board to stop sending data
            //
        }

        /// <summary>
        /// EndConnectionToArduino.
        /// </summary>
        private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = _port.ReadExisting();
            _lastData += data;
            string[] allData = _lastData.Split('\n');
            if (allData.Length > 2)
            {
                _lastData = allData[allData.Length - 2];
                _lastData = _lastData.Trim();
                _rotationAngle = MathHelper.Pi * Convert.ToSingle(_lastData) / 100.0f;
            }

            //if (_stopped) return;
            //// Send data received from the Arduino to the Listbox on the UI thread
            //Application.Current.Dispatcher.BeginInvoke((Action)(() =>
            //{
            //    string data = _port.ReadExisting();
            //    _lastData += data;
            //    string[] allData = _lastData.Split('\n');
            //    _lastData = allData[allData.Length - 1];
            //    foreach (var word in allData.Take(allData.Length - 1))
            //    {
            //        SerialDataViewModel.Items.Add(word);
            //    }
            //}));
        }

        // Specify what you want to happen when the Elapsed event is raised.
        private static void _port_DataReceived_timer(object source, ElapsedEventArgs e)
        {
            Console.WriteLine("Arduino data simulation!");
        }

        #endregion

        #region IDemoScreen Members

        public string GetTitle()
        {
            return "Body with a single fixture";
        }

        public string GetDetails()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("This demo shows a single body with one attached fixture and shape.");
            sb.AppendLine("A fixture binds a shape to a body and adds material");
            sb.AppendLine("properties such as density, friction, and restitution.");
            sb.AppendLine(string.Empty);
            sb.AppendLine("GamePad:");
            sb.AppendLine("  - Rotate object: left and right triggers");
            sb.AppendLine("  - Move object: right thumbstick");
            sb.AppendLine("  - Move cursor: left thumbstick");
            sb.AppendLine("  - Grab object (beneath cursor): A button");
            sb.AppendLine("  - Drag grabbed object: left thumbstick");
            sb.AppendLine("  - Exit to menu: Back button");
            sb.AppendLine(string.Empty);
            sb.AppendLine("Keyboard:");
            sb.AppendLine("  - Rotate Object: left and right arrows");
            sb.AppendLine("  - Move Object: A,S,D,W");
            sb.AppendLine("  - Exit to menu: Escape");
            sb.AppendLine(string.Empty);
            sb.AppendLine("Mouse / Touchscreen");
            sb.AppendLine("  - Grab object (beneath cursor): Left click");
            sb.AppendLine("  - Drag grabbed object: move mouse / finger");
            return sb.ToString();
        }

        #endregion

        public override void LoadContent()
        {
            base.LoadContent();

            World.Gravity = Vector2.Zero;

            Texture2D polygonTexture = ScreenManager.Content.Load<Texture2D>("Arm/robot_arm_arm");

            _border = new Border(World, ScreenManager, Camera);

            _rectangle = BodyFactory.CreateRectangle(World, 5f, 5f, 1f);
            _rectangle.BodyType = BodyType.Dynamic;

            SetUserAgent(_rectangle, 100f, 100f);


            // create sprite based on body
            _rectangleSprite = new Sprite(polygonTexture);//ScreenManager.Assets.TextureFromShape(_rectangle.FixtureList[0].Shape, MaterialType.arm, Color.Orange, 1f));
            //_armSprite = new Sprite();
            
            //Connect to arduino and start listening 
            StartConnectionToArduino();
        }

        public override void Draw(GameTime gameTime)
        {
            ScreenManager.SpriteBatch.Begin(0, null, null, null, null, null, Camera.View);
            ScreenManager.SpriteBatch.Draw(_rectangleSprite.Texture, ConvertUnits.ToDisplayUnits(_rectangle.Position), null, Color.White, _rectangle.Rotation, _rectangleSprite.Origin, 1f, SpriteEffects.None, 0f);
            ScreenManager.SpriteBatch.End();
            _border.Draw();
            base.Draw(gameTime);
        }

        public override void Update(GameTime gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
        {
            // The time since Update was called last.
            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;

            //// TODO: Add your game logic here.
            //_rotationAngle += elapsed;
            //float circle = MathHelper.Pi * 2;
            //_rotationAngle = _rotationAngle % circle;

            _rectangle.Rotation = _rotationAngle;

            base.Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);            
        }
    }
}